# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

BMC64 is a bare-metal Commodore emulator (C64, C128, VIC20, Plus/4, PET) for Raspberry Pi. It runs directly on the hardware with no OS — the kernel boots as a bare-metal Circle application. VICE handles the actual emulation; Circle/circle-stdlib provide the hardware abstraction layer.

This fork of [randyrossi/bmc64](https://github.com/randyrossi/bmc64) has two primary goals:

1. **Update VICE to 3.9** — the upstream project uses VICE 3.3; this fork upgrades to VICE 3.9 (released 2024-12-24) which adds the binary remote monitor protocol (port 6502), ethernet/TFE emulation, and six years of emulation improvements.
2. **Support Raspberry Pi Zero 2W** — primary test target is a Pi Zero 2W (BCM2710A1, quad-core Cortex-A53). Uses the Pi 3 code path, not the single-core Pi Zero path.

Pi 5 support is deferred — it requires mandatory 64-bit (AArch64) and a complete rewrite of the display and audio subsystems.

## Current Status

**The build system currently targets VICE 3.3 (original upstream state).** The VICE 3.9 upgrade is in progress — the source is vendored and the arch/raspi layer ported, but the build system wiring has not yet been applied. See "Work In Progress" below.

## Build System

### Prerequisites

- ARM cross-compiler: `gcc-arm-none-eabi` (GNU Embedded Toolchain for Arm)
- Host tools: `git build-essential automake autoconf libtool pkg-config autoconf-archive autotools-dev xa65`

### Environment Setup

```bash
export ARM_HOME=/path/to/gcc-arm-none-eabi
export PATH=$PATH:$ARM_HOME/bin
export ARM_VERSION=9.2.1
# If toolchain binaries use "none" prefix:
export PREFIX=arm-none-eabi-
```

### Build Steps (must run in order)

```bash
./clean_all.sh                       # Clean everything (required before first build)
./make_all.sh [pi0|pi2|pi3|pi4|pizero2w]
./make_machines.sh [pi0|pi2|pi3|pi4|pizero2w]
```

`make_all.sh` will fail if `clean_all.sh` hasn't been run first. `make_machines.sh` will fail if `make_all.sh` hasn't been run first.

### Output Kernels

| Pi Model    | Board arg   | Kernel filename   |
|-------------|-------------|-------------------|
| Pi Zero     | `pi0`       | `kernel.img`      |
| Pi 2        | `pi2`       | `kernel7.img`     |
| Pi 3        | `pi3`       | `kernel8-32.img`  |
| Pi Zero 2W  | `pizero2w`  | `kernel8-32.img`  |
| Pi 4        | `pi4`       | `kernel7l.img`    |

`make_machines.sh` appends a machine suffix: `.c64`, `.c128`, `.vic20`, `.plus4`, `.plus4emu`, `.pet`

### Building a single machine

```bash
BOARD=pizero2w make -f Makefile-C64
```

Makefile variants: `Makefile-C64`, `Makefile-C128`, `Makefile-VIC20`, `Makefile-Plus4`, `Makefile-Plus4Emu`, `Makefile-PET`

### Pi Zero 2W notes

`pizero2w` uses the same Circle `--raspberrypi=3` configuration and ARM Cortex-A53 compile flags as `pi3`. It produces `kernel8-32.img` and does **not** use `RASPI_LITE`, teensy-resid, or `BMC64_REPORT_THROTTLE`. A `RASPI_ZERO2W` preprocessor define distinguishes it from pi3. Copy `kernel8-32.img.c64` → `kernel8-32.img` on the SD card to boot.

### Pi 4 notes

`pi4` uses `kernel7l.img`. The CRT shader is disabled at runtime via `allow_shader()` which checks `circle_get_model() <= 3`. The `RASPI_PI4` define guards `OGLInit()` in `fbl.cpp` to prevent an EGL initialization crash on VideoCore VI.

### Known-good dependency hashes (circle-stdlib subtree)

If patches fail to apply, reset these submodule dirs with `git reset HASH --hard`:
- `third_party/circle-stdlib`: `dda16112cdb5470240cd51fb33bf72b311634340`
- `third_party/circle-stdlib/libs/circle`: `fe24b6bebd1532f2a0ee981af12eaf50cc9e97fb`
- `third_party/circle-stdlib/libs/circle-newlib`: `c01f95bcb08278d9e00f9795c7641284d4f89931`
- `third_party/circle-stdlib/libs/mbedtls`: `d81c11b8ab61fd5b2da8133aa73c5fe33a0633eb`

There are no unit tests — the only way to test is to deploy to hardware.

## Work In Progress

### Board support changes needed in build system

All of the following changes need to be applied. They were designed and validated in session but the build system files were reverted. Apply them together:

**`make_all.sh`** — add `pizero2w` board arm (mirrors `pi3` exactly: same Circle `--raspberrypi=3`, same `-march=armv8-a -mtune=cortex-a53` flags, no `RASPI_LITE`, no `BMC64_REPORT_THROTTLE`).

**`make_machines.sh`** — add `pizero2w` → `kernel8-32.img`.

**All `Makefile-C64/C128/VIC20/Plus4/PET`** — inside the `ifeq ($(BOARD),pi0)` / `else` block, add:
```makefile
ifeq ($(BOARD),pizero2w)
CFLAGS += "-DRASPI_ZERO2W"
endif
ifeq ($(BOARD),pi4)
CFLAGS += "-DRASPI_PI4"
endif
```

**`third_party/common/Makefile`** — add the same `RASPI_ZERO2W` and `RASPI_PI4` defines for `CFLAGS_FOR_TARGET`.

**`third_party/common/menu.c`** — in the about screen block (around the `RASPI_LITE` guard), add:
```c
#ifdef RASPI_LITE
  ui_menu_add_button(MENU_TEXT, about_root, "For the Raspberry Pi Zero");
#elif defined(RASPI_ZERO2W)
  ui_menu_add_button(MENU_TEXT, about_root, "For the Raspberry Pi Zero 2W");
#elif defined(RASPI_PI4)
  ui_menu_add_button(MENU_TEXT, about_root, "For the Raspberry Pi 4");
#else
  ui_menu_add_button(MENU_TEXT, about_root, "For the Raspberry Pi 2/3");
#endif
```

**`fbl.cpp`** — guard `OGLInit()` in `FrameBufferLayer::Initialize()` to prevent EGL crash on Pi 4's VideoCore VI:
```cpp
bcm_host_init();
#ifndef RASPI_PI4
OGLInit();
#endif
dispman_display_ = vc_dispmanx_display_open(0);
```

### VICE 3.9 upgrade — build system wiring needed

VICE 3.9 source is fully vendored at `third_party/vice-3.9/` with the `arch/raspi/` platform layer ported. The following build system changes still need to be applied:

**`make_all.sh`** and **`make_machines.sh`**: change `vice-3.3` → `vice-3.9` everywhere.

**`make_all.sh`** VICE configure invocations:
- Change `--disable-sdlui --disable-sdlui2` → `--disable-sdl1ui --disable-sdl2ui --disable-gtk3ui` (flag rename in 3.9)
- Add `--enable-ethernet` to all five board configure calls
- Replace `cd src && make libarchdep` with separate steps:
  ```bash
  cd src/arch/raspi && make && cd ../../..
  cd src/arch/shared && make && cd ../../..
  cd src && make libhvsc && cd ..
  ```
  (In VICE 3.9, `libarchdep.a` moved to `src/arch/shared/` and is separate from `src/arch/raspi/libarch.a`)

**All `Makefile-C64/C128/VIC20/Plus4/PET`**:
- Remove `$(VICE)/usleep.o` and `$(VICE)/libm_math.o` (these files no longer exist in VICE 3.9)
- Add `$(VICE)/arch/shared/libarchdep.a` before `$(VICE)/arch/raspi/libarch.a` in `VICELIBS`

**`Makefile`** (main): change `third_party/vice-3.3/src` include path → `third_party/vice-3.9/src`.

**`clean_all.sh`**: change `vice-3.3` → `vice-3.9`.

### VICE 3.9 arch/raspi — what's already done

`third_party/vice-3.9/src/arch/raspi/` contains the ported platform layer:

- **`missing.c`** — fully updated for VICE 3.9 API: `ui_init()` signature fixed, DDraw/DX9/SDL stubs removed, new stubs added (`kbd_arch_init/shutdown`, `kbd_hotkey_init/shutdown`, `ui_pause_*`, `vsyncarch_advance_frame`, `video_arch_get_active_chip`, `uimon_petscii_out` and related binary monitor functions, all `vsid_ui_*` SID player stubs)
- **`rawnetarch.c`** — new file implementing the full `rawnet_arch_*` interface for bare-metal. Transmit/receive are stubs with TODO comments pointing to Circle's `CNetSubSystem`. The driver enumeration and resource init functions include the new VICE 3.9 additions (`rawnet_arch_enumdriver_*`, `rawnet_arch_resources_init/shutdown`).
- **`Makefile.am`** — `rawnetarch.c` added to `libarch_a_SOURCES`
- **`configure.ac`** — patched to add `--enable-raspiui` / `--enable-raspilite` flags, `ARCH_DIR`, `RASPI_COMPILE` define, and raspi Makefile entries alongside `--enable-headlessui`

Key API changes from VICE 3.3 → 3.9 already handled in `missing.c`:
- `ui_init(int*, char**)` → `ui_init(void)` + new `ui_init_with_args(int*, char**)`
- `palette_load(file, palette)` → `palette_load(file, subpath, palette)`
- `usleep.o` and `libm_math.o` removed from VICE core (no replacement needed)
- `arch/shared/libarchdep.a` is now separate from `arch/raspi/libarch.a`

**What likely still needs fixing in `vice_api.c` and `videoarch.c`**: these large files (~1300 and ~530 lines) were not yet compiled against VICE 3.9 headers. Run `make_all.sh pizero2w` and fix compile errors as they appear — each error is a concrete pointer to a specific API change.

## Architecture

### Layered Design

```
main.cpp
  └─ CKernel (kernel.h/cpp)          ← Circle bare-metal kernel entry point
       └─ ViceApp (viceapp.h/cpp)     ← Hardware init: USB, SD card, GPIO, audio
            └─ ViceEmulatorCore       ← Runs VICE on a separate CPU core (multicore Pi)
            or Plus4EmulatorCore      ← Alternative for Plus4Emu (Pi3 only)
```

- **`main.cpp`**: Entry point; instantiates `CKernel` and calls `Run()`.
- **`kernel.h/cpp`**: Circle kernel. Handles USB keyboard/mouse input, GPIO scanning, menu navigation, video frame callbacks. `static_kernel` is a global singleton.
- **`viceapp.h/cpp`**: Initializes all Circle hardware subsystems (SD card via EMMC, USB HCI, GPIO manager, scheduler, sound). Owns the `ViceEmulatorCore`.
- **`viceemulatorcore.h/cpp`**: Runs VICE on a secondary ARM core via `CMultiCoreSupport`. Manages timing (cycles per second), ReSID filter computation, and the emulation loop.
- **`plus4emulatorcore.h/cpp`**: Same role but using Plus4Emu instead of VICE. Pi3 only.
- **`viceoptions.h/cpp`**: Parses `cmdline.txt` boot parameters (machine timing, video mode, audio output, GPIO config, etc.).
- **`vicesound.h/cpp`** + **`vicesoundbasedevice.h/cpp`**: VCHIQ audio output to Pi GPU sound hardware.
- **`vicescreen.h/cpp`**: Framebuffer management and CRT shader integration.
- **`fbl.h/cpp`**: Framebuffer layer — manages the Pi's GPU framebuffer, scaling, borders. When `uses_shader_` is true, renders via OpenGL ES/EGL (Pi 0–3 only). `allow_shader()` in `emux_api.c` returns false for Pi 4+ (`circle_get_model() <= 3`).
- **`crt_pi_idx.c/h`**, **`crt_pi_rgb.c/h`**: CRT shader implementations (indexed and RGB modes).

### third_party/common — The VICE↔Circle Interface

`third_party/common/` is the critical glue layer between VICE (C) and Circle (C++):
- **`circle.h`**: Declares the interface that VICE calls into the kernel and vice versa.
- **`emux_api.h/c`**: Emulator API — abstracts machine-specific calls (used by the menu/UI layer to work across C64/C128/VIC20/etc.). Contains `allow_shader()` which gates CRT shader by Pi model.
- **`menu.c/h`** and `menu_*.c/h`: The entire in-emulator OSD menu system, written in C.
- **`ui.c/h`**: On-screen display and UI rendering.
- **`kbd.c/h`**, **`joy.c/h`**: Keyboard and joystick input routing.
- **`overlay.c/h`**: Status bar / overlay rendering.

### Machine Variants

Each machine (C64, C128, VIC20, Plus4, PET) has its own `Makefile-*` that sets `MACHINE_CLASS` (e.g. `RASPI_C64`) and links the appropriate VICE sub-libraries. The `MACHINE_CLASS` define propagates throughout the source as `#if defined(RASPI_C64)` guards. Plus4Emu uses a separate `MACHINE_CLASS=RASPI_PLUS4EMU` and `Plus4EmulatorCore` instead of `ViceEmulatorCore`.

### VICE Integration

VICE 3.9 lives in `third_party/vice-3.9/` (3.3 kept in `third_party/vice-3.3/` for reference). The `arch/raspi/` subdirectory contains all Raspberry Pi–specific platform implementations:

- **`arch/raspi/archdep.c`** — file system paths, process stubs
- **`arch/raspi/vice_api.c`** — the main emulator ↔ Circle bridge (~1300 lines); implements `emux_*` API called by `third_party/common/`
- **`arch/raspi/videoarch.c`** — video canvas, palette handling, dispmanx blit
- **`arch/raspi/missing.c`** — stubs for VICE APIs not needed in bare-metal
- **`arch/raspi/rawnetarch.c`** — Ethernet raw frame interface (Circle CNetSubSystem integration TODO)
- **`arch/raspi/{c64,c128,vic20,plus4,pet}/`** — per-machine menu and video arch overrides

### Video Pipeline

1. VICE renders into an internal framebuffer via its normal VIC-II/VDC code.
2. `vicescreen` / `fbl` copies/scales that into the Pi GPU framebuffer via dispmanx.
3. Optional CRT shader (`crt_pi_rgb` / `crt_pi_idx`) is applied via OpenGL ES (Pi 0–3 only).
4. Frame timing is locked to the GPU vsync signal to achieve true 50/60 Hz.

### GPIO Configs

Five GPIO configurations are supported (set via `gpio_config` in `settings.txt` or the menu). Config 2 (full keyboard + joystick via PCB) uses a specific pin matrix defined in `viceapp.h`. Config 4 (userport outputs) requires `enable_gpio_outputs=true` in `cmdline.txt` and must never be used with the keyboard PCB attached.

## SD Card Notes

### JiffyDOS

JiffyDOS ROM files are installed in `bmc64-sdcard/C64/`:
- `jiffydos_kernal` — JiffyDOS 64 Kernal 6.01 (sourced from `~/Downloads/JiffyDOS_C64_Kernal_6.01.rom`)
- `jiffydos_1541` — JiffyDOS 1541 DOS 5.0 (sourced from `~/Downloads/JiffyDOS_1541_5.0.rom`)

Activated via `bmc64-sdcard/vice.ini` under `[C64]`:
```
KernalName=jiffydos_kernal
DosName1541ii=jiffydos_1541
Drive8Type=1541II
```

The JiffyDOS 1541 ROM is the **1541-II variant** (ROM starts with `$97`). Use `DosName1541ii` and `Drive8Type=1541II`, not `DosName1541`/`Drive8Type=1541` — VICE validates the ROM signature against the drive type and will reject a 1541-II ROM loaded as a plain 1541.

ROM filenames in `vice.ini` must be bare names (no path prefix) — BMC64 resolves them relative to the machine ROM directory (`/C64/` on the SD card).

The reference `vice.ini` is tracked at `sdcard/vice.ini` in this repo.

Files cannot be transferred to the SD card over the network (BMC64 is bare-metal, no TFTP or network daemon). Physical SD card removal and remounting on the host is required.

*Also 2026-05-30 — Added JiffyDOS support to bmc64-sdcard: copied ROM files to C64/ directory, configured vice.ini with KernalName=jiffydos_kernal, DosName1541ii=jiffydos_1541, Drive8Type=1541II (1541-II variant required). Tracked reference copy at sdcard/vice.ini.*

*Also 2026-05-26 — Fixed three bugs: (1) Dual-disk attach hang: `archdep_real_path_equal` (`third_party/vice-3.9/src/arch/shared/archdep_real_path.c`) allocated two `char[PATH_MAX]` (8KB total) on VICE's stack for a comparison that reduces to `strcmp` on bare metal — stack overflow on second attachment when the duplicate-check loop first has a non-null path. Fix: RASPI_COMPILE guard uses `strcmp` directly. (2) Menu hang after disk attach: `vc_dispmanx_update_submit_sync` in `ui_render_single_frame` (`third_party/common/ui.c`) deadlocked when VCHIQ audio pipeline was saturated after first disk attached. Fix: async dispmanx submit (sync=0) with 50fps `circle_sleep` rate-limiter. (3) Safe-boot recovery: at SD card mount in `ViceStdioApp::Initialize` (`viceapp.cpp`), if GPIO23 or GPIO19 (fire button, either joystick bank) is held, copy `kernel8-32-safe.img` over `kernel8-32.img` and reboot. `bmc_sync_to_pi.sh --kernel` now backs up current kernel to `kernel8-32-safe.img` before overwriting. Both kernel slots written on every deploy.*
