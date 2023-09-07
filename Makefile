
MAKEFLAGS += --no-builtin-rules

asm = tools\nasm\nasm
gcc = D:\Files\Programs\i686-elf-tools\bin\i686-elf-gcc
ld = D:\Files\Programs\i686-elf-tools\bin\i686-elf-ld

qemu = D:\Files\Programs\qemu\qemu-system-i386
dd = tools\dd

BUILD_DIR = build
FOLDERS = kernel libc

KERNEL_C    := $(wildcard kernel/*.c kernel/*/*.c libc/*.c libc/*/*.c)
KERNEL_ASM  := $(wildcard kernel/*.asm kernel/*/*.asm libc/*.asm libc/*/*.asm)
KERNEL_OBJ  := $(KERNEL_C:.c=.o) $(KERNEL_ASM:.asm=.o)
KERNEL_OBJ  := $(patsubst %,$(BUILD_DIR)/%,$(KERNEL_OBJ))
KERNEL_DEPS := $(KERNEL_OBJ:.o=.d)

BOOT_DEPS := $(BUILD_DIR)/boot.d


run: $(BUILD_DIR)/amkos.img
	$(qemu) -accel hax -hda $<

-include $(BOOT_DEPS)
-include $(KERNEL_DEPS)

$(BUILD_DIR)/amkos.img: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	$(asm) amkos.asm -f bin -o $@
	$(dd) if=$(BUILD_DIR)/boot.bin of=$@ seek=0
	$(dd) if=$(BUILD_DIR)/kernel.bin of=$@ seek=1

$(BUILD_DIR)/kernel.bin : $(BUILD_DIR)/kernel/kernel_entry.o $(KERNEL_OBJ)
	$(ld) -o $@ -Ttext 0x10000 $^ --oformat binary

$(BUILD_DIR)/boot.bin : bootloader/boot.asm
	$(asm) -f bin -MD $(@:.bin=.d) $< -o $@

$(BUILD_DIR)/%.o : %.asm
	$(asm) -f elf -MD $(@:.o=.d) $< -o $@

$(BUILD_DIR)/%.o : %.c
	$(gcc) -I. -MD -ffreestanding -c $< -o $@

clean:
	-rmdir /s /q $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(FOLDERS:%=$(BUILD_DIR)\\%)
