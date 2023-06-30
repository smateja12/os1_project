# os1_project

The OS kernel is implemented for the RISC V architecture and runs in using the qemu emulator. There are no external librarires that were used for OS kernel implementation, which means that no host operating system calls are allowed (threads, memory allocation, etc.)

OS kernel display:
![image](https://github.com/smateja12/os1_project/assets/74740382/25033b4f-abaf-4c51-8a5c-b2717e5a1619)

The ABI (application binary interface) serves as a binary interface for system calls, accessed through software interrupts. It facilitates the transfer of arguments to system calls via processor registers, enables mode switching from system to user, and facilitates entry into kernel code.

The C API (application programming interface) is a traditional, procedural programming interface for system calls, implemented as C functions. These functions act as a layer of abstraction for the underlying ABI interface.

The C++ API is an object-oriented API that acts as a wrapper for the functions provided by the C API.

| Number | Function                                                                                                    | Description                                                                                                                                                                                                                                                                   |
| :----- | :---------------------------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0x01   | `void* allocate_memory(size_t size);`                                                                      | Allocates a block of memory with a size of `size`, rounded and aligned to the nearest multiple of `MEM_BLOCK_SIZE`. Returns a pointer to the allocated memory block if successful, or null otherwise.                                                                                       |
| 0x02   | `int free_memory(void* ptr);`                                                                               | Frees the memory previously allocated with `allocate_memory`. Returns 0 upon success, or a negative value (error code) otherwise. The argument `ptr` must be a valid pointer returned by `allocate_memory`. If `ptr` is not a valid pointer, the behavior is undefined: the kernel might return an error or behave unpredictably. |
| 0x11   | `class _thread; typedef _thread* thread_t; int create_thread(thread_t* handle, void(*start_routine)(void*), void* arg);` | Creates a new thread that executes the `start_routine` function with the argument `arg`. Upon success, the `handle` will contain the identifier for the created thread, and the return value will be 0. Otherwise, a negative value is returned. The `handle` is used to identify threads.                                      |
| 0x12   | `int exit_thread();`                                                                                         | Terminates the currently active thread. Returns a negative value in case of an error.                                                                                                                                                                                        |
| 0x13   | `void dispatch_thread();`                                                                                    | Potentially switches the processor from the active thread to another thread.                                                                                                                                                                                                 |
| 0x21   | `class _sem; typedef _sem* sem_t; int open_semaphore(sem_t* handle, unsigned init);`                            | Creates a semaphore with an initial value of `init`. Upon success, the `handle` will contain the identifier for the semaphore, and the return value will be 0. Otherwise, a negative value is returned. The `handle` is used to identify semaphores.                             |
| 0x22   | `int close_semaphore(sem_t handle);`                                                                          | Frees the semaphore identified by `handle`. All threads that were blocked on this semaphore will be unblocked, and their `wait` operation will return an error. Returns 0 upon success, or a negative value otherwise.                                                                 |
| 0x23   | `int wait_semaphore(sem_t id);`                                                                               | Waits for the semaphore identified by `id`. Returns 0 upon success. Even if the semaphore is deallocated while the active thread is waiting, a negative value is returned.                                                                                                                                                                |
| 0x24   | `int signal_semaphore(sem_t id);`                                                                             | Signals the semaphore identified by `id`. Returns 0 upon success, or a negative value otherwise.                                                                                                                                                                             |
| 0x31   | `typedef unsigned long time_t; int sleep_time(time_t);`                                                      | Suspends the active thread for a specified number of timer periods. Returns 0 upon success, or a negative value otherwise.                                                                                                                                                 |
| 0x41   | `const int EOF = -1; char read_char();`                                                                      | Reads a character from the input buffer, which is filled from the console. If the buffer is empty, the active thread is suspended until a character becomes available. Returns the read character if successful, or EOF otherwise.                                                                                                             |
| 0x42   | `void write_char(char);`                                                                                      | Writes the specified character
