# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

BMC64 is a bare-metal Commodore emulator (C64, C128, VIC20, Plus/4, PET) for Raspberry Pi. It runs directly on the hardware with no OS — the kernel boots as a bare-metal Circle application. VICE handles the actual emulation; Circle/circle-stdlib provide the hardware abstraction layer.

This fork of [randyrossi/bmc64](https://github.com/randyrossi/bmc64) has two primary goals:

1. **Update VICE to 3.9** — the upstream project uses VICE 3.3; this fork upgrades to VICE 3.9 (released 2024-12-24) which adds the binary remote monitor protocol (port 6502), ethernet/TFE emulation, and six years of emulation improvements.
2. **Support Raspberry Pi Zero 2W** — primary test target is a Pi Zero 2W (BCM2710A1, quad-core Cortex-A53). Uses the Pi 3 code path, not the single-core Pi Zero path.

Pi 5 support is deferred — it requires mandatory 64-bit (AArch64) and a complete rewrite of the display and audio subsystems.

## Current Status

**VICE 3.9 C64 build is fully working on Pi Zero 2W.** C64-only. Other machine types are out of scope for this fork.

Working:
- C64 boots to READY screen ✓
- Display stable (NTSC, 720p@60Hz HDMI) ✓
- Audio working (HDMI, 6581 SID) ✓
- GPIO keyboard PCB (gpio_config=1) working ✓
- USB keyboard working ✓
- Positional keymap (rpi_pos.vkm) as default ✓
- SwiftLink/ACIA virtual modem for TCP BBS — Hayes AT commands, DNS resolution ✓
- TFE ethernet (CS8900A) menu toggle ✓ (for future use)
- About screen credits fork and original repo ✓
- SMSC951x (LAN9514) onboard ethernet working ✓ — ping, TFTP, TCP all confirmed
- Network Info menu item showing IP/MAC ✓
- TFTP server on port 69 — upload/download files to SD card over the network ✓
- Lavender/black menu color scheme ✓

**Out of scope**: binary remote monitor, Contiki/TFE networking, C128/VIC20/Plus4/PET.

**Remaining work**:
- Test SwiftLink TCP BBS end-to-end (StrikeTerm dialing bbs.retrocampus.com:6510)
- Remove debug `circle_log`/`circle_logf` calls from VICE source before shipping

**RTL8153 / AX88772 drivers** — written but blocked (see below). Not needed since SMSC951x works.

**USB devices must be connected before power-on** (BMC64 limitation — no hot-plug support)

**Key fixes**:
- `MachineVideoStandard=1` (NTSC) in vice.ini — was PAL-N, caused audio drift/freeze
- `audio_out=hdmi` in cmdline.txt — forces VCHIQ to HDMI (default Auto missed it)
- `SidModel=0` + `SidStereo=0` in vice.ini — single 6581 chip
- Wall-clock rate limiter in `ViceSound::AddChunk` as audio backstop
- `KeymapIndex` default set to `KBD_INDEX_POS` in `keymap.c` line 1473
- `resources_set_int` must NOT be called from `emu_machine_init()` — resource system not yet initialized at that point
- SMSC951x TX uses BSS-section static variables (`s_pTxURB`, `s_bTxPending`, `s_bTxBusy`, `s_nTxFrameLen`) for cross-core visibility — Cortex-A53 AArch32 ACTLR is RAZ/WI so heap volatile is not reliable across cores
- Full libusb.a rebuild required after smsc951x.h changes to avoid sizeof heap corruption

## Network Configuration

Static IP: `192.168.80.164/24`, gateway `192.168.80.1`, DNS `192.168.80.1`.

Set in `CNetInitTask::Run()` in `kernel.cpp`. TFTP server starts automatically after net init.

**TFTP usage** (from host, SD card must be inserted in Pi):
```bash
# Download a file from the Pi:
curl -o localfile.ini tftp://192.168.80.164/vice.ini

# Upload a file to the Pi:
# (requires tftp client or curl with PUT support)
```
Files are read/written relative to SD card root (`/`).

**SwiftLink virtual modem** (`third_party/vice-3.9/src/arch/raspi/rs232-raspi-dev.c`):
- Hayes AT commands: `ATZ` → OK, `ATDT host:port` → TCP connect, `ATH` → disconnect
- Hostnames resolved via `CDNSClient` (DNS server = gateway)
- `CONNECT 38400` response on successful dial
- StrikeTerm config: SwiftLink modem, ANSI terminal, 38400 baud, Hayes protocol

**Test BBS**: `ATDT bbs.retrocampus.com:6510`

## Build System

### Prerequisites

- ARM cross-compiler: `gcc-arm-none-eabi` (GNU Embedded Toolchain for Arm)
- Host tools: `git build-essential automake autoconf libtool pkg-config autoconf-archive autotools-dev xa65`

### Environment Setup

```bash
export ARM_HOME=/usr
export PATH=$PATH:$ARM_HOME/bin
export ARM_VERSION=14.2.1
export PREFIX=arm-none-eabi-
```

### Build Steps (must run in order)

```bash
./clean_all.sh                       # Clean everything (required before first build)
./make_all.sh pizero2w
./make_machines.sh pizero2w
```

`make_all.sh` will fail if `clean_all.sh` hasn't been run first. `make_machines.sh` will fail if `make_all.sh` hasn't been run first.

Only C64 is built: `MACHINES="C64:c64"` in `make_machines.sh`.

### Output Kernels

| Pi Model    | Board arg   | Kernel filename   |
|-------------|-------------|-------------------|
| Pi Zero     | `pi0`       | `kernel.img`      |
| Pi 2        | `pi2`       | `kernel7.img`     |
| Pi 3        | `pi3`       | `kernel8-32.img`  |
| Pi Zero 2W  | `pizero2w`  | `kernel8-32.img`  |
| Pi 4        | `pi4`       | `kernel7l.img`    |

`make_machines.sh` appends `.c64` suffix. Copy `kernel8-32.img.c64` → `kernel8-32.img` on SD card.

### Pi Zero 2W notes

`pizero2w` uses the same Circle `--raspberrypi=3` configuration and ARM Cortex-A53 compile flags as `pi3`. It produces `kernel8-32.img` and does **not** use `RASPI_LITE`, teensy-resid, or `BMC64_REPORT_THROTTLE`. A `RASPI_ZERO2W` preprocessor define distinguishes it from pi3.

### Pi 4 notes

`pi4` uses `kernel7l.img`. The CRT shader is disabled at runtime via `allow_shader()` which checks `circle_get_model() <= 3`. The `RASPI_PI4` define guards `OGLInit()` in `fbl.cpp` to prevent an EGL initialization crash on VideoCore VI.

### Known-good dependency hashes (circle-stdlib subtree)

If patches fail to apply, reset these submodule dirs with `git reset HASH --hard`:
- `third_party/circle-stdlib`: `dda16112cdb5470240cd51fb33bf72b311634340`
- `third_party/circle-stdlib/libs/circle`: `fe24b6bebd1532f2a0ee981af12eaf50cc9e97fb`
- `third_party/circle-stdlib/libs/circle-newlib`: `c01f95bcb08278d9e00f9795c7641284d4f89931`
- `third_party/circle-stdlib/libs/mbedtls`: `d81c11b8ab61fd5b2da8133aa73c5fe33a0633eb`

There are no unit tests — the only way to test is to deploy to hardware.

## VICE 3.9 Upgrade — Completed Changes

All build system and source changes are applied. Key changes across files:

### Build System (`make_all.sh`, `make_machines.sh`, `clean_all.sh`, Makefiles)
- `vice-3.3` → `vice-3.9` throughout
- `pizero2w` board support added (mirrors pi3: `--raspberrypi=3`, Cortex-A53 flags)
- VICE configure flags: `--disable-sdl1ui --disable-sdl2ui --disable-gtk3ui --enable-raspiui --disable-ethernet --without-libcurl --without-png --without-zlib`
- Build steps after `make x64`: build `arch/raspi`, `arch/shared`, `hvsc` separately
- `Makefile-C64`: `libarch.a` now links **before** `libarchdep.a` (so raspi overrides win over shared defaults)
- `Makefile-C64`: removed `usleep.o`, `libm_math.o`; added `userport`, `datasette`, `imagecontents`, `libzmbv`, `arch/shared/libarchdep.a`, `arch/shared/hotkeys`

### `viceemulatorcore.cpp`
- Updated includes: vice-3.3 → vice-3.9
- Removed `ComputeSamplingTable` calls (removed from VICE 3.9)
- `ComputeResidFilter` is a no-op stub (Filter no longer takes model arg)
- Removed `sid_job*` multicore SID loop (removed from VICE 3.9)
- **argv**: removed `-soundsync 0` and `-refresh 1` (both removed from VICE 3.9); argc=5

### `third_party/vice-3.9/src/arch/raspi/archdep.c`
- `archdep_default_sysfile_pathlist()` returns `"/"` (SD card root)
  - VICE 3.9's `sysfile_load()` now takes a `subpath` arg (e.g. `"C64"`, `"DRIVES"`) that `findpath()` appends to each path component, building `/C64/kernal` etc.
  - Previous VICE 3.3 version returned `/C64:/DRIVES:` which would double-path to `/C64/C64/kernal`

### `third_party/vice-3.9/src/arch/shared/archdep_access.c`
- Added `RASPI_COMPILE` implementation using `fopen`/`fclose`
  - Default `#else` stub returned -1 always (file never found)
  - `access()` in newlib always returns 0 (file always "found", wrong path used)
  - `fopen` actually checks existence on Circle's FAT32

### `third_party/vice-3.9/src/c64/c64-resources.c`
- `KernalName` default: `"kernal"` (was `C64_KERNAL_REV3_NAME = "kernal-901227-03.bin"`)
- `BasicName` default: `"basic"` (was `C64_BASIC_NAME = "basic-901226-01.bin"`)
- `ChargenName` default: `"chargen"` (was `C64_CHARGEN_NAME = "chargen-901225-01.bin"`)
- `kernal_match[]` table: all entries map to `"kernal"` (was versioned names like `"kernal-901227-03.bin"`)
  - `KernalRevision` resource setter calls `resources_set_string("KernalName", name)` using this table — it was overriding our default with the versioned name

### `third_party/vice-3.9/src/arch/raspi/videoarch.c`
- In `video_canvas_refresh`: removed `circle_show_fbl(layer)` call during init
  - `circle_show_fbl` during `raster_realize` (init phase) crashes via `vc_dispmanx_update_start` assert
  - `emux_ensure_video()` in `vsyncarch_postsync` already handles show/hide correctly once emulator is running

### Other VICE 3.9 API changes (already applied in prior sessions)
- `missing.c`: stubs for all new VICE 3.9 APIs
- `videoarch.c`: removed `video_draw_buffer_callback_s`, added depth=8, keyboard/joystick mod params
- `vice_api.c`: `datasette_control(0,cmd)`, `file_system_attach_disk(unit,0,file)`, `joyport_io_sim_set_potx/y`
- `archdep.c`: rewritten for VICE 3.9 API
- `archdep.h`: added required defines (`ARCHDEP_DIR_SEP_CHR`, etc.)

### Debug Logging System
`kernel.cpp` has a `circle_log`/`circle_logf` system that:
- Writes to serial (UART)
- Accumulates messages in a 32KB RAM buffer
- Writes entire buffer to `/VICE39_DEBUG.txt` on SD card on every call (using `fopen("w")`)

This logging is spread throughout VICE 3.9 init code for debugging. **Remove all `circle_logf` debug calls before shipping** (they're in `main.c`, `init.c`, `c64.c`, `c64rom.c`, `vicii/vicii.c`, `raster/raster.c`, `videoarch.c`).

## Architecture

### Layered Design

```
main.cpp
  └─ CKernel (kernel.h/cpp)          ← Circle bare-metal kernel entry point
       └─ ViceApp (viceapp.h/cpp)     ← Hardware init: USB, SD card, GPIO, audio
            └─ ViceEmulatorCore       ← Runs VICE on a separate CPU core (multicore Pi)
```

- **`main.cpp`**: Entry point; instantiates `CKernel` and calls `Run()`.
- **`kernel.h/cpp`**: Circle kernel. Handles USB keyboard/mouse input, GPIO scanning, menu navigation, video frame callbacks. `static_kernel` is a global singleton.
- **`viceapp.h/cpp`**: Initializes all Circle hardware subsystems (SD card via EMMC, USB HCI, GPIO manager, scheduler, sound). Owns the `ViceEmulatorCore`.
- **`viceemulatorcore.h/cpp`**: Runs VICE on a secondary ARM core via `CMultiCoreSupport`. Manages timing (cycles per second), ReSID filter computation, and the emulation loop.
- **`viceoptions.h/cpp`**: Parses `cmdline.txt` boot parameters (machine timing, video mode, audio output, GPIO config, etc.).
- **`vicesound.h/cpp`** + **`vicesoundbasedevice.h/cpp`**: VCHIQ audio output to Pi GPU sound hardware.
- **`fbl.h/cpp`**: Framebuffer layer — manages the Pi's GPU framebuffer, scaling, borders. `FrameBufferLayer::Show()` calls `vc_dispmanx_update_start` which must NOT be called during VICE init (before `vsyncarch_postsync` runs). `allow_shader()` in `emux_api.c` returns false for Pi 4+ (`circle_get_model() <= 3`).
- **`crt_pi_idx.c/h`**, **`crt_pi_rgb.c/h`**: CRT shader implementations.
- **`tftpserver.h/cpp`**: TFTP server — subclasses Circle's `CTFTPDaemon`, maps filenames to SD card paths. Started by `CNetInitTask` after network init.

### third_party/common — The VICE↔Circle Interface

`third_party/common/` is the critical glue layer between VICE (C) and Circle (C++):
- **`circle.h`**: Declares the interface that VICE calls into the kernel and vice versa. Includes TCP socket API (`circle_net_tcp_new`, `circle_net_tcp_connect`, `circle_net_send`, `circle_net_recv`, `circle_net_can_recv`, `circle_net_close`) and network info (`circle_net_get_ip_str`, `circle_net_get_mac`).
- **`emux_api.h/c`**: Emulator API — abstracts machine-specific calls. `emux_ensure_video()` handles `circle_show_fbl` / `circle_hide_fbl` based on `vic_enabled`/`vic_showing` state — called each frame from `vsyncarch_postsync`.
- **`menu.c/h`** and `menu_*.c/h`: The entire in-emulator OSD menu system, written in C.
- **`ui.c/h`**: On-screen display and UI rendering.
- **`kbd.c/h`**, **`joy.c/h`**: Keyboard and joystick input routing.
- **`overlay.c/h`**: Status bar / overlay rendering.

### SMSC951x Ethernet Driver

`third_party/circle-stdlib/libs/circle/lib/usb/smsc951x.cpp` — heavily modified from upstream Circle.

Key design: **Core 0** handles all USB operations (IRQ, URB submission). **Core 1** runs VICE and calls `SendFrame`. Cross-core state uses BSS-section statics (not heap members) because Cortex-A53 AArch32 ACTLR is RAZ/WI — `volatile` heap pointers are not reliably visible across cores.

- `s_pTxURB`, `s_bTxPending`, `s_bTxBusy`, `s_nTxFrameLen` — TX state in BSS
- `NetKernelTimer` fires on Core 0 every ~10ms, calls `RearmRxURB()` + `SubmitTxURB()`
- `ReceiveFrame()` is non-blocking (async URB + `m_bRxDone` flag poll)
- TX URB pre-allocated in `Configure()`, never deleted, reset via `ResetForReuse()` each TX
- `DataMemBarrier()` around `s_bTxPending` writes/reads

### VICE 3.9 Video Pipeline

In VICE 3.9, the draw_buffer_callback mechanism was removed. VICE allocates its own internal draw buffer in `raster.c` via `raster_realize_frame_buffer`. Our arch code:

1. `video_canvas_refresh` is called each frame (and once during init from `video_viewport_resize`)
2. On first call: allocates Circle FBL (via `alloc_circle_fbl_for_canvas`) if `draw_buffer_width > 0`
3. Each frame: `memcpy` from VICE's draw_buffer → Circle FBL pixel buffer
4. `vsyncarch_postsync` → `emux_ensure_video()` → `circle_show_fbl` (first frame only)
5. `vsyncarch_postsync` → `circle_frames_ready_fbl` → dispmanx swap → GPU vsync wait

### VICE 3.9 sysfile / ROM loading

- `archdep_default_sysfile_pathlist("C64")` returns `"/"`
- `sysfile_load("kernal", "C64", buf, size, size)` → `findpath("kernal", "/", "C64", R_OK)` → `/C64/kernal` ✓
- `sysfile_load(rom, "DRIVES", buf, ...)` → `/DRIVES/rom` ✓
- ROM names default to bare names (`kernal`, `basic`, `chargen`) matching SD card layout
- `archdep_access` uses `fopen` to check file existence (newlib `access()` always returns 0)

### GPIO Configs

Five GPIO configurations are supported (set via `gpio_config` in `settings.txt` or the menu). Config 2 (full keyboard + joystick via PCB) uses a specific pin matrix defined in `viceapp.h`. Config 4 (userport outputs) requires `enable_gpio_outputs=true` in `cmdline.txt` and must never be used with the keyboard PCB attached.
