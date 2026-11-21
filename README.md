# Terbine OS UEFI v0.0.1

Educational x86_64 UEFI bootloader written from scratch in C, with a freestanding C++ kernel. **No GRUB**. The `nosktlnfc.exe` file is a custom `TBK1` image (not a Windows PE executable).

## openSUSE Tumbleweed prerequisites

```bash
sudo zypper install clang lld qemu-x86 ovmf python3
```

Package names may differ by repository; check `zypper search` if necessary. QEMU executable: `qemu-system-x86_64`. Requires `clang`, `lld-link`, `ld.lld`, Python 3 and OVMF firmware.

## Build and run

```bash
./build.sh
./run.sh
```

The QEMU terminal (`-serial stdio`) should print:

```text
[Terbine] nosktlnfc.exe started!
[Terbine] Custom UEFI bootloader -> custom C++ kernel
[Terbine] ExitBootServices succeeded.
[Terbine] v0.0.1 ready (serial output only).
```

The bootloader's earlier UEFI console messages may not be visible in `-display none` mode. To view UEFI screen output, use `-display gtk` and change `-serial stdio` to `-serial stdio` (the serial remains in terminal). The final kernel output is serial only.

## Layout

- `src/efi.h`: minimal UEFI definitions
- `src/boot.c`: PE/COFF UEFI bootloader, reads `\\nosktlnfc.exe` from same ESP, exits boot services, jumps to kernel
- `src/kernel.cpp`: bare-metal C++ kernel, COM1 serial output
- `src/kernel.ld`: raw kernel layout
- `tools/pack.py`: prepends 16-byte TBK1 header
- `esp/EFI/BOOT/BOOTX64.EFI`: built bootloader
- `esp/nosktlnfc.exe`: built custom kernel image

## Important limitations

- Proof of concept for QEMU, **not safe for real hardware**.
- No paging ownership, memory manager, interrupt handlers, framebuffer, filesystem drivers, keyboard or shell yet.
- Assumes firmware-provided identity mappings continue to work long enough for the initial kernel. A production kernel must create and own its page tables and stack.
- Kernel size limited to <1 MiB; no kernel relocation support, no BSS initialization beyond current image assumptions.
- No Secure Boot signing, verified kernel image, or full robust error recovery.
- The UEFI loader is an EFI application (PE/COFF), but the kernel named `.exe` is not Windows PE.
