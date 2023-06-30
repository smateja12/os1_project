# os1_project

The OS kernel is implemented for the RISC V architecture and runs in using the qemu emulator. There are no external librarires that were used for OS kernel implementation, which means that no host operating system calls are allowed (threads, memory allocation, etc.)

OS kernel display:


The ABI (application binary interface) serves as a binary interface for system calls, accessed through software interrupts. It facilitates the transfer of arguments to system calls via processor registers, enables mode switching from system to user, and facilitates entry into kernel code.

The C API (application programming interface) is a traditional, procedural programming interface for system calls, implemented as C functions. These functions act as a layer of abstraction for the underlying ABI interface.

The C++ API is an object-oriented API that acts as a wrapper for the functions provided by the C API.

