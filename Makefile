# ======================================================
# Compiler / Linker
# ======================================================

CC      := gcc
AS      := nasm
LD      := ld
OBJCOPY := objcopy
OBJDUMP := objdump
QEMU 	:= qemu-system-x86_64
NM 		:= nm

CFLAGS  := 	-m32 \
			-std=c11 \
			-ffreestanding \
			-g \
			-O0 \
           	-fno-pic \
		   	-fno-pie \
			-fno-stack-protector \
			-fno-builtin \
			-fno-omit-frame-pointer \
		   	-no-pie \
           	-Wall \
		   	-Wextra \
			-Wpedantic \
			-Wshadow \
			-Wconversion \
			-Wundef \
			-Wmissing-prototypes \
			-Wstrict-prototypes \
			-Werror \
			-MMD \
			-MP

LDFLAGS := 	-m elf_i386 \
			-T kernel/linker.ld \
			-nostdlib \
			-Map=kernel.map

# ======================================================
# Source discovery
# ======================================================

C_SOURCES := $(shell find kernel drivers -type f -name '*.c')

ASM_SOURCES := $(filter-out kernel/kernel_entry.asm, \
	$(shell find kernel -type f -name '*.asm'))

HEADERS := $(shell find kernel drivers -type f -name '*.h')

# ======================================================
# Objects
# ======================================================

C_OBJ := $(C_SOURCES:.c=.o)
ASM_OBJ := $(ASM_SOURCES:.asm=.o)

OBJ := $(C_OBJ) $(ASM_OBJ)

# ======================================================
# Final targets
# ======================================================

all: os-image

run: os-image
	qemu-system-i386 \
		-drive format=raw,file=os-image,if=floppy

debug: os-image
	qemu-system-i386 \
		-drive format=raw,file=os-image,if=floppy \
		-s -S

# ======================================================
# Kernel
# ======================================================

kernel.elf: kernel/kernel_entry.o $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

os-image: boot/boot_sect.bin kernel.bin
	cp kernel.bin kernel.padded.bin

	size=$$(stat -c%s kernel.padded.bin); \
	padded=$$((( size + 511 ) / 512*512)); \
	truncate -s $$padded kernel.padded.bin
	cat boot/boot_sect.bin kernel.padded.bin > $@

# ======================================================
# Compilation rules
# ======================================================

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $< -f elf -o $@

%.bin: %.asm
	$(AS) -I boot/ $< -f bin -o $@

# ======================================================
# Dependencies
# ======================================================

-include $(C_OBJ:.o=.d)

# ======================================================
# Utilities
# ======================================================

clean:
	find kernel -name "*.o" -delete
	find kernel -name "*.d" -delete
	find drivers -name "*.o" -delete
	find drivers -name "*.d" -delete

	rm -f boot/*.bin
	rm -f kernel.bin
	rm -f kernel.elf
	rm -f os-image
	rm -f kernel.disasm
	rm -f kernel.map
	rm -f kernel.padded.bin

symbols: kernel.elf
	$(NM) -n kernel.elf

sections: kernel.elf
	$(OBJDUMP) -h kernel.elf

disassemble: kernel.elf
	$(OBJDUMP) -d -M intel -S kernel.elf > kernel.disasm

size: kernel.elf
	size kernel.elf

.PHONY: all run debug clean symbols sections disassemble size