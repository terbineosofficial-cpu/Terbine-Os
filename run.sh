#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
OVMF="${OVMF_CODE:-}"
if [[ -z "$OVMF" ]]; then
 for p in /usr/share/edk2/ovmf/OVMF_CODE.fd /usr/share/OVMF/OVMF_CODE_4M.fd /usr/share/OVMF/OVMF_CODE.fd /usr/share/edk2-ovmf/x64/OVMF_CODE.fd; do
  if [[ -f "$p" ]]; then OVMF="$p"; break; fi
 done
fi
if [[ -z "$OVMF" ]]; then echo 'OVMF firmware not found. Set OVMF_CODE=/path/to/OVMF_CODE.fd'; exit 1; fi
qemu-system-x86_64 -machine q35 -m 256M -drive if=pflash,format=raw,readonly=on,file="$OVMF" -drive format=raw,file=fat:rw:esp -serial stdio -display none -monitor none -no-reboot
