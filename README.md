# AMK-OS
A **very incomplete** operating system **(simple bootloader & kernel)**.  
Made for intel x86 Architecture, for educational purposes.  
**NOT TESTED** on real hardware and **USELESS**.  
  
## Features:
- **Bootloader.**  
- **Kernel. [In Progress]**  
  
## Build & Run
Edit **makefile** and change (assembler & compiler) paths to where you installed them.  
And also, I'm running on Windows, you should change the commands to create the empty disk if you're running on differnt OS.  
```
fsutil file createnew amkos.img 8388608   // for Windows (creates empty 8MB disk space)  
```
Now to compile & run, just type `make run` in the command prompt.  
  
**Build Requirements :**  
- [NASM](https://www.nasm.us/) Assembler x86.  
- [GNU C](https://wiki.osdev.org/GCC_Cross-Compiler) Cross-compiler for x86.  
- [Make utility](https://www.gnu.org/software/make/) (packaged with GCC).  
  
**Run Requirements :**
- [QEMU](https://www.qemu.org/) emulator.  
  