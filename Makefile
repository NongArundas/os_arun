CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -c -ffreestanding -fno-pic
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld

# Ensure the build directory exists
all: prep build/arun_os.elf

prep:
	mkdir -p build

# Look inside the build folder for dependencies
build/arun_os.elf: build/loader.o build/kernel.o
	$(LD) $(LDFLAGS) build/loader.o build/kernel.o -o build/arun_os.elf

# Look inside the kernel folder for the C source file
build/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) kernel/kernel.c -o build/kernel.o

# Look inside the boot folder for the Assembly source file
build/loader.o: boot/loader.s
	$(AS) $(ASFLAGS) boot/loader.s -o build/loader.o

clean:
	rm -rf build

run: all
	qemu-system-i386 -kernel build/arun_os.elf
