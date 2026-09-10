# Arun OS

A small 32-bit bare-metal operating system kernel written in C and x86 assembly.

## Requirements

- GCC with 32-bit support
- NASM
- GNU `ld`
- QEMU for running the kernel
- Git

## Build

Build the kernel image with:

```bash
make
```

The output is written to `build/arun_os.elf`.

## Run

Start the kernel in QEMU with:

```bash
make run
```

Remove generated build files with:

```bash
make clean
```

## Manual Git Push

After making daily changes, commit and push them manually:

```bash
git add .
git commit -m "Daily progress"
git push origin main
```

Generated files under `build/` are excluded from Git.
