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
