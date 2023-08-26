
MAKEFLAGS += --no-builtin-rules

asm = tools\nasm\nasm
gcc = D:\Files\Programs\i686-elf-tools\bin\i686-elf-gcc
ld = D:\Files\Programs\i686-elf-tools\bin\i686-elf-ld

qemu = D:\Files\Programs\qemu\qemu-system-i386
dd = tools\dd

BUILD_DIR = build
FOLDERS = bootloader kernel

KERNEL_C := $(wildcard kernel/*.c kernel/*/*.c)
KERNEL_ASM := $(wildcard kernel/*.asm kernel/*/*.asm)
KERNEL_OBJ := $(KERNEL_C:.c=.o) $(KERNEL_ASM:.asm=.o)
KERNEL_OBJ := $(patsubst %,$(BUILD_DIR)/%,$(KERNEL_OBJ))
KERNEL_DEPS := $(KERNEL_OBJ:.o=.d)

BOOT_ASM := $(wildcard bootloader/*.asm bootloader/*/*.asm)
BOOT_DEPS := $(patsubst %,$(BUILD_DIR)/%,$(BOOT_ASM))
BOOT_DEPS := $(BOOT_DEPS:.asm=.d)


run: amkos.img
	$(qemu) -accel hax -hda amkos.img

-include $(BOOT_DEPS)
-include $(KERNEL_DEPS)

amkos.img: $(BUILD_DIR)/bootloader/boot.bin $(BUILD_DIR)/kernel.bin
	$(dd) if=$(BUILD_DIR)/bootloader/boot.bin of=$@ seek=0
	$(dd) if=$(BUILD_DIR)/kernel.bin of=$@ seek=1

$(BUILD_DIR)/kernel.bin : $(BUILD_DIR)/bootloader/kernel_entry.o $(KERNEL_OBJ)
	$(ld) -o $@ -Ttext 0x1000 $^ --oformat binary

$(BUILD_DIR)/%.bin : %.asm
	$(asm) -f bin -MD $(@:.bin=.d) $< -o $@

$(BUILD_DIR)/%.o : %.asm
	$(asm) -f elf -MD $(@:.o=.d) $< -o $@

$(BUILD_DIR)/%.o : %.c
	$(gcc) -ffreestanding -c $< -o $@

clean:
	rmdir /s /q $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	mkdir $(FOLDERS:%=$(BUILD_DIR)\\%)
