
MAKEFLAGS += --no-builtin-rules

qemu = D:\Files\Programs\qemu\qemu-system-i386
dd = Tools\dd
src = src
asm = Tools\nasm\nasm

ASM_SRC = $(wildcard $(src)/*.asm)
C_SRC = $(wildcard $(src)/*.c)
OBJS = $(ASM_SRC:.asm=.o) $(C_SRC:.c=.o)
DEPS = $(OBJS:.o=.d)

run: boot.bin
	$(qemu) -accel hax -hda amkos.img

-include $(DEPS)

boot.bin: $(src)\boot.asm
	$(asm) -f bin -i$(src)/ -MD $(<:.asm=.d) $< -o $@
	$(dd) if=boot.bin of=amkos.img seek=0

clean:
	del $(src)\*.d
	del boot.bin

