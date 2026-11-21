#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p build esp/EFI/BOOT
CLANG=${CLANG:-clang}
LLD_LINK=${LLD_LINK:-lld-link}
LD_LLD=${LD_LLD:-ld.lld}
"$CLANG" -target x86_64-pc-windows-msvc -ffreestanding -fno-stack-protector -fshort-wchar -mno-red-zone -Wall -Wextra -O2 -c src/boot.c -o build/boot.obj
"$LLD_LINK" /subsystem:efi_application /entry:efi_main /nodefaultlib /machine:x64 /out:esp/EFI/BOOT/BOOTX64.EFI build/boot.obj
"$CLANG" -target x86_64-unknown-none-elf -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -fno-pic -fno-pie -mno-red-zone -mno-sse -mno-sse2 -O2 -c src/kernel.cpp -o build/kernel.o
"$LD_LLD" -T src/kernel.ld --oformat=binary -o build/kernel.bin build/kernel.o
python3 tools/pack.py build/kernel.bin esp/nosktlnfc.exe
printf 'Built ESP directory: %s/esp\n' "$PWD"
