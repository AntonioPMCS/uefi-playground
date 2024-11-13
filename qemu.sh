#!/bin/bash
                   qemu-system-x86_64 -bios Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd -drive file=fat:rw:Build/MdeModule/DEBUG_GCC5,format=raw      -device e1000,netdev=n0 -netdev user,id=n0                 -nographic                   -debugcon file:debug.log -global isa-debugcon.iobase=0x402
