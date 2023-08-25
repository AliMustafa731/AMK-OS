
MAKEFLAGS += --no-builtin-rules

qemu = D:\Files\Programs\qemu\qemu-system-i386
dd = Tools\dd
asm = Tools\nasm\nasm

ASM_SRC = $(wildcard */*.asm)
C_SRC = $(wildcard */*.c)
OBJS = $(ASM_SRC:.asm=.o) $(C_SRC:.c=.o)
DEPS = $(OBJS:.o=.d)

run: amkos.img
	$(qemu) -accel hax -hda amkos.img

-include $(DEPS)

amkos.img: boot.bin
	$(dd) if=boot.bin of=amkos.img seek=0

boot.bin: bootloader\boot.asm
	$(asm) -f bin -ibootloader/ -MD $(<:.asm=.d) $< -o $@

clean:
	del bootloader\*.d
	del boot.bin

