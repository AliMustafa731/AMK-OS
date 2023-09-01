## AMK-OS
Operating System Development for the Intel x86 Architecture, for educational purposes.  
**NOT TESTED** on real hardware and **USELESS**.  
  
## Features:
- **Bootloader.**  
- **Kernel. [In Progress]**  
  
## Build & Run
Edit **Makefile** and change (assembler & compiler) paths to where you installed them.  
I'm running on Windows, if you're running on differnt OS you should change the commands to create the empty disk.  
```
fsutil file createnew amkos.img 8388608   // for Windows (creates empty 8MB disk space)  
```
To compile & run, type `make run` in the command prompt.  
  
**Build Requirements :**  
- [NASM](https://www.nasm.us/) Assembler for x86.  
- [GNU C](https://wiki.osdev.org/GCC_Cross-Compiler) Cross-compiler for x86.  
- [Make utility](https://www.gnu.org/software/make/) (packaged with GCC).  
  
**Run Requirements :**  
- [QEMU](https://www.qemu.org/) emulator.  
  