# NebulaOS (privacy-focused UEFI x64 live system)

NebulaOS now includes a **privacy-first profile** with a custom built-in VPN app (`Nebula Tunnel`) and strict privacy defaults after installation.

## Privacy-focused behavior

- Live boot includes apps: Browser, Notes, Files, VPN, Installer.
- First install boot writes persistent flags:
  - `NebulaInstalled=1`
  - `NebulaPrivacyStrict=1`
  - `NebulaVpnEnabled=1`
- Next boots hide Installer and keep privacy mode + VPN enabled.

## Custom VPN app

- App name: **Nebula Tunnel**
- EXU kind: `kind=5`
- Entry profile: `vpn://secure-mode`
- Included in both runtime embedded app list and `apps/vpn.exu`.

## Build commands

```bash
make help
make all
make esp
```

Full ISO (POSIX/MSYS2/WSL with FAT+ISO tools installed):

```bash
make iso
```

## Windows note

In native `cmd.exe`, `make all` and `make esp` are supported by the Windows branch of the Makefile.
For `make iso`, use MSYS2 or WSL with:

- `xorriso`
- `mtools`
- `dosfstools`

## Artifacts

- `build/BOOTX64.EFI`
- `build/esp/EFI/BOOT/BOOTX64.EFI`
- `build/nebulaos-x64-uefi.iso` (from POSIX/MSYS2/WSL `make iso`)
