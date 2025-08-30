CC = i386-elf-gcc
AS = i386-elf-as
LD = i386-elf-ld

KERNEL_DIR = kernel

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(KERNEL_DIR)/include
LDFLAGS = -T linker.ld -nostdlib

SRC_DIR = $(KERNEL_DIR)/src

SRCS = $(shell find $(SRC_DIR) -type f \( -name "*.c" -o -name "*.asm" \))

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))

KERNEL_ELF = $(BUILD_DIR)/sigmaos-kernel.elf

ISO_DIR = $(BUILD_DIR)/iso

ISO = $(BUILD_DIR)/sigmaos.iso

all: iso

$(ISO): $(KERNEL_ELF)
	rm -rf $(ISO_DIR) && mkdir -p $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp grub.cfg $(ISO_DIR)/boot/grub
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/sigmaos-kernel.elf
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO)

iso: $(ISO)

kernel: $(KERNEL_ELF)

$(KERNEL_ELF): $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@[ -d $(dir $@) ] || (echo "mkdir -p $(dir $@)"; mkdir -p $(dir $@))
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.asm.o: $(SRC_DIR)/%.asm | $(OBJ_DIR)
	@[ -d $(dir $@) ] || (echo "mkdir -p $(dir $@)"; mkdir -p $(dir $@))
	$(AS) -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: kernel clean run iso
