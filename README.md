# SxS OS
A dive-in-at-the-deep-end education in the nitty-gritty workings of an Operating System.

> "What I cannot create I do not understand"

All code is heavily commented to provide an educational commentary of exactly what is going on. Please dive in.

### Features
- A custom-written but fairly standard boot loader
- A screen driver for outputting to the screen
- A port driver for communicating with various I/O chips: PIT, PIC, the screen etc
- Excpetion handlers
- Interrupt handlers, supporting a timer & keyboard input

### Requirements

Requires qemu to be installed:

On Mac:
```
brew install qemu
```

You'll also need these cross compilers so that you can compile for your target.
On Mac you can get them pre-built.
```sh
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc i386-elf-gdb
```

### Build & Run
- `make run` to run the Operating System in QEMU

- `make run-debug` to run with debugger listener.
Execution will halt until the debugger is attached.
Run the `QEMU attach` debug configuration included in the `.vscode/launch.json`.
The process execution will begin and you now have debug capability from your editor.

### Misc
In the tools folder you'll find some useful resources for further explanation of all of this.

There are also some scripts for disassembly which are useful too.
