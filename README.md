# NebulaOS (UEFI x64 live system + installer flow)

NebulaOS now boots as a **live system** with built-in apps and a built-in installer app. After installation is persisted, the installer app no longer appears.

## Live vs installed behavior

- **Live mode (first boot):**
  - Browser, Notes, Files, and Installer apps are shown.
  - Installer writes a persistent non-volatile UEFI variable `NebulaInstalled=1`.
- **Installed mode (next boots):**
  - Browser, Notes, and Files apps are shown.
  - Installer app is removed from the app list and desktop shortcuts.

## Technical implementation

- Persistence uses UEFI Runtime Services:
  - `GetVariable` checks install state.
  - `SetVariable` writes install state.
- Installer app kind is `kind=4` and is loaded only when install state is missing.

## Display compatibility

Mode selection prioritizes common resolutions:

- 1920x1080
- 1366x768
- 1600x900
- 1280x720
- 1280x800
- 1024x768
- 800x600
- 640x480

Also supports GOP pixel formats:

- RGB 8:8:8
- BGR 8:8:8
- Bit-mask modes

## Build

```bash
make
```

## Prepare ESP tree

```bash
make esp
```

Outputs:

- `build/BOOTX64.EFI`
- `build/esp/EFI/BOOT/BOOTX64.EFI`
