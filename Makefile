
MAKEFLAGS += --no-builtin-rules

qemu = D:\Files\Programs\qemu\qemu-system-i386
dd = Tools\dd
src = src
dist = build
asm = Tools\nasm\nasm

deps=$(wildcard $(src)/*.d)

run: build
	$(qemu) -accel hax -hda amkos.img

-include $(deps)

build: boot.bin

boot.bin: $(src)\boot.asm
	$(asm) -f bin -i$(src)/ -MD $(src)\boot.d $(src)\boot.asm -o boot.bin
	$(dd) if=boot.bin of=amkos.img seek=0

clean:
	del $(src)\*.d
	del boot.bin

