# NebulaOS (UEFI x64 live system + installer flow)

NebulaOS boots as a live UEFI system with built-in apps and a built-in installer app. After installation is persisted, the installer app disappears on later boots.

## Live vs installed behavior

- **Live mode (first boot):** Browser, Notes, Files, Installer.
- **Installed mode (next boots):** Browser, Notes, Files (installer hidden).
- Persistence uses non-volatile UEFI variable `NebulaInstalled=1`.

## Commands

```bash
make help
make all
make esp
```

Linux/MSYS2/WSL full ISO:

```bash
make iso
```

## Why you saw those errors in `cmd.exe`

From your log:

- `clang ... failed` -> LLVM not in PATH.
- `rm -rf build ... failed` -> Unix clean command in old Makefile.
- `No rule to make target 'iso'` -> older copy of project.

This version fixes those by:

- adding a Windows-native `cmd.exe` Makefile branch,
- adding `make help`,
- showing clear tool errors with `make check-tools`.

## Windows setup (cmd.exe)

1. Install LLVM (must provide `clang` + `lld-link` in PATH).
2. Open new `cmd.exe`.
3. Run:

```bat
make help
make all
make esp
```

If you need a **full ISO** on Windows, run in either:

- **MSYS2 shell** with `xorriso`, `mtools`, `dosfstools`, or
- **WSL** (recommended) and run `make iso` there.

Artifacts:

- `build/BOOTX64.EFI`
- `build/esp/EFI/BOOT/BOOTX64.EFI`
- `build/nebulaos-x64-uefi.iso` (from `make iso` on POSIX-like shell)
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
