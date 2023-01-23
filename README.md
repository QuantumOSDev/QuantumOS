# QuantumOS

## About
A simple operating system written in C and Assembly.

## Project Map
- `boot`: Multiboot assembly code
- `clib`: C-Library (aiming for POSIX-like userland)
- `config`: Compilation time configuration like linker config
- `core`: Source for data-structures, stdio, stdlib, string
- `drivers`: Source for drivers, e.g mouse, keyboard
- `fs`: Source for ATA drivers and Filesystem implementations
- `include`: All includes except C-Lib
- `kernel`: All kernel-side sources including IDT, IRQ, GDT assembly
- `net`: Networking implementations
- `sys`: All system sources
- `userspace`: Userspace sources, e.g syscalls

## Todo List
- [] Implement ATA Drivers
- [] Reimplement ACPI
- [] Reimplement PCI
- [] Custom bootloader
- [] Implement EXT2 and FAT32
- [] Implement IWL-Wifi drivers
- [] Implement sound-card drivers
- [] More...

# Screenshots
![](/screenshots/screenshot1.png)
![](/screenshots/F9838711-A50F-44C9-8CC5-C9C84AD35576.jpeg)


