# BMC64 — VICE 3.9 / Pi Zero 2W Fork

This is a fork of [randyrossi/bmc64](https://github.com/randyrossi/bmc64) with two primary goals:

1. **VICE 3.9** — upgrade from VICE 3.3 to VICE 3.9 (released 2024-12-24), gaining the binary remote monitor protocol, ethernet/TFE emulation, and six years of emulation improvements.
2. **Raspberry Pi Zero 2W** — primary test target is the Pi Zero 2W (BCM2710A1, quad-core Cortex-A53). Uses the Pi 3 code path.

**C64 only.** C128, VIC20, Plus/4, and PET are out of scope for this fork.

---

## Status

**Fully working on Pi Zero 2W and Pi 3:**

- C64 boots to READY screen
- Display — NTSC, 720p@60Hz HDMI
- Audio — HDMI, 6581 SID (reSID, Fast Resampling)
- GPIO keyboard PCB (gpio_config=1) and USB keyboard
- Positional keymap (`rpi_pos.vkm`) as default
- Cartridge loading (CRT, 8K, 16K, Ultimax)
- Disk and tape loading
- SwiftLink/ACIA virtual modem — Hayes AT commands, DNS, TCP BBS
- TFE ethernet (CS8900A) menu toggle
- SMSC951x (LAN9514) onboard Ethernet — ping, TFTP, TCP confirmed
- TFTP server on port 69 — upload/download files to SD card over the network

---

## Network

Static IP: `192.168.80.164/24`, gateway/DNS: `192.168.80.1`.  
Set in `CNetInitTask::Run()` in `kernel.cpp`.

**TFTP** (SD card must be inserted):

```bash
# Download a file from the Pi:
curl -o localfile.ini tftp://192.168.80.164/vice.ini

# Upload a file to the Pi:
curl --upload-file myfile.d64 tftp://192.168.80.164/disks/C64/myfile.d64
```

**SwiftLink virtual modem** — enable `Acia1Enable=1` in `vice.ini`:

```
ATZ          → OK
ATDT host:port → TCP connect, responds CONNECT 38400
ATH          → disconnect
```

Test BBS: `ATDT bbs.retrocampus.com:6510`

---

## Build

### Prerequisites

```bash
# Packages
sudo apt install gcc-arm-none-eabi build-essential automake autoconf libtool \
    pkg-config autoconf-archive autotools-dev xa65

# Environment (add to ~/.bashrc or set before building)
export ARM_HOME=/usr
export PATH=$PATH:$ARM_HOME/bin
export ARM_VERSION=14.2.1
export PREFIX=arm-none-eabi-
```

### Steps (must run in order)

```bash
./clean_all.sh           # required before first build
./make_all.sh pizero2w
./make_machines.sh pizero2w
```

Output kernel: `kernel8-32.img` (Pi 3 / Pi Zero 2W).  
Copy `kernel8-32.img` to the SD card root as both `kernel8-32.img` and `kernel8-32.img.c64`.

### Supported board targets

| Board arg   | Kernel filename   | Hardware              |
|-------------|-------------------|-----------------------|
| `pizero2w`  | `kernel8-32.img`  | Pi Zero 2W, Pi 3      |
| `pi4`       | `kernel7l.img`    | Pi 4 (CRT shader off) |

---

## SD Card Layout

```
/
├── config.txt          arm_64bit=0, gpu_mem=128
├── cmdline.txt         fast=true machine_timing=ntsc-hdmi scaling_params=... audio_out=hdmi
├── vice.ini            SidModel=0 SidStereo=0 Acia1Enable=1 KeymapIndex=1
├── kernel8-32.img      boot kernel
├── kernel8-32.img.c64  same file, required for machine switching
├── C64/
│   ├── kernal
│   ├── basic
│   ├── chargen
│   ├── d1541II
│   ├── rpi_pos.vkm
│   └── rpi_maxi_pos.vkm
├── DRIVES/
│   └── (drive ROMs)
├── disks/
│   └── C64/            .d64, .g64, etc.
├── tapes/
│   └── C64/            .tap, .t64
├── carts/
│   └── C64/            .crt, .bin
└── snapshots/
    └── C64/
```

File browser looks in `/disks/C64`, `/carts/C64`, `/tapes/C64` by default (configurable via "Files Location Convention" in the Prefs menu — the alternative is `/C64/disks`, `/C64/carts`, etc.).

---

## Key Configuration

**`cmdline.txt`:**
```
fast=true machine_timing=ntsc-hdmi scaling_params=384,240,1152,720 audio_out=hdmi
```

**`vice.ini`** (important settings for this fork):
```
MachineVideoStandard=1    ; NTSC — PAL-N causes audio drift/freeze
SidModel=0                ; 6581
SidStereo=0
Acia1Enable=1             ; SwiftLink virtual modem
KeymapIndex=1             ; positional keymap
KeymapUserPosFile="rpi_maxi_pos.vkm"
```

---

## Known Limitations

- **No hot-plug** — all USB devices must be connected before power-on.
- **C64 only** — other machine types are not built or supported in this fork.
- **Pi 5 not supported** — requires AArch64 and a full display/audio rewrite.
- USB gamepad support is limited; not all gamepads will work.

---

## Keyboard

- **F12** — open/close menu
- **ESC / RUNSTOP** — exit menu
- **Commodore Key + F7** — alternate menu toggle (real keyboard)

| Usage         | Mapping           |
|---------------|-------------------|
| USB keyboard  | Positional (`rpi_pos.vkm`) |
| GPIO PCB      | Positional        |
| TheC64 Maxi   | `rpi_maxi_pos.vkm` |

---

## GPIO Configs

| Config | Description                          |
|--------|--------------------------------------|
| 1      | Menu nav buttons + real joysticks    |
| 2      | Real keyboard + real joysticks (PCB) |
| 3      | Waveshare Game HAT                   |
| 4      | Userport outputs + joysticks (dangerous — see below) |
| 5      | Custom defined                       |

Set via `gpio_config=N` in `settings.txt` (0-indexed: Config 2 = `gpio_config=1`).

**Config 4 warning:** capable of driving GPIO pins as 3.3V outputs. Never use with the keyboard PCB attached — direct pin-to-pin shorts will damage the GPIO.  Requires `enable_gpio_outputs=true` in `cmdline.txt` to unlock from the menu.

---

## Video

Scaling and timing work the same as upstream BMC64 — see the [upstream README](https://github.com/randyrossi/bmc64/blob/master/README.md) for full detail on custom HDMI modes, integer scaling, DPI, and the CRT shader. Key points for this fork:

- NTSC (`machine_timing=ntsc-hdmi`) is the tested/recommended mode.
- CRT shader is automatically disabled on Pi 4+ (`circle_get_model() > 3`).
- `scaling_params=384,240,1152,720` gives 3x integer scaling at 720p.

---

## Audio

- ReSID, Fast Resampling, 6581 model.
- `audio_out=hdmi` in `cmdline.txt` is required — auto-detection misses HDMI on Pi Zero 2W.
- A wall-clock rate limiter in `ViceSound::AddChunk` prevents the GPU audio buffer from filling when VICE runs slightly faster than real-time (~0.1% on Pi Zero 2W).

---

## Upstream Project

For pre-compiled images, changelog, and full documentation for the multi-machine upstream build:  
[https://github.com/randyrossi/bmc64](https://github.com/randyrossi/bmc64)  
[https://accentual.com/bmc64](https://accentual.com/bmc64)
