//
// Created by os on 9/2/22.
//

#include "../h/syscall_c.h"

void* mem_alloc (size_t size)
{
    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
    size_t numOfBlocks = ((size - 1) / MEM_BLOCK_SIZE) + 1;
    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

    __asm__ volatile("mv a1, %0" : : "r" ((uint64)numOfBlocks));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));
    __asm__ volatile("ecall");
    void* ptr;
    __asm__ volatile("mv %0, a0" : "=r" (ptr));
    return ptr;
}

int mem_free (void* ptr)
{
    __asm__ volatile("mv a1, %0" : : "r"((uint64)ptr));
    __asm__ volatile("mv a0, %0" : : "r"(0x02));
    __asm__ volatile("ecall");
    int retval;
    __asm__ volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
)
{
    uint64* stack;
    if (start_routine)
    {
        // ovde moramo da alociramo memoriju preko naseg MemoryAllocator-a, ne preko sistemskog poziva
//        stack = new uint64[DEFAULT_STACK_SIZE];
        size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
        size_t numOfBlocks = ((DEFAULT_STACK_SIZE - 1) / MEM_BLOCK_SIZE) + 1;
        if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

        stack = (uint64*)MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    }else
    {
        stack = nullptr;
    }
    // slanje argumenata u prekidnu rutinu
    __asm__ volatile("mv a4, %0" : : "r"((uint64) handle));
    __asm__ volatile("mv a3, %0" : : "r"((uint64) start_routine));
    __asm__ volatile("mv a2, %0" : : "r"((uint64) arg));
    __asm__ volatile("mv a1, %0" : : "r"((uint64) stack));
    __asm__ volatile("mv a0, %0" : : "r"(0x11));
    __asm__ volatile("ecall");
    int retval;
    __asm__ volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

int thread_exit()
{
    __asm__ volatile("mv a0, %0" : : "r"(0x12));
    __asm__ volatile("ecall");
    int retval;
    __asm__ volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

void thread_dispatch()
{
    __asm__ volatile("mv a0, %0" : : "r"(0x13));
    __asm__ volatile("ecall");
}

int thread_start(thread_t* handle)
{
    __asm__ volatile("mv a1, %0" : : "r"((uint64) handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x14));
    __asm__ volatile("ecall");
    int retval;
    __asm__ volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

int thread_init(
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
)
{
    uint64* stack;
    if (start_routine)
    {
        // ovde moramo da alociramo memoriju preko naseg MemoryAllocator-a, ne preko sistemskog poziva
//        stack = new uint64[DEFAULT_STACK_SIZE];
        size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
        size_t numOfBlocks = ((DEFAULT_STACK_SIZE - 1) / MEM_BLOCK_SIZE) + 1;
        if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

        stack = (uint64*)MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    }else
    {
        stack = nullptr;
    }
    // slanje argumenata u prekidnu rutinu
    __asm__ volatile("mv a4, %0" : : "r"((uint64) handle));
    __asm__ volatile("mv a3, %0" : : "r"((uint64) start_routine));
    __asm__ volatile("mv a2, %0" : : "r"((uint64) arg));
    __asm__ volatile("mv a1, %0" : : "r"((uint64) stack));
    __asm__ volatile("mv a0, %0" : : "r"(0x15));
    __asm__ volatile("ecall");
    int retval;
    __asm__ volatile("mv %0, a0" : "=r"(retval));
    return retval;
}

char getc()
{
    __asm__ volatile("mv a0, %0" : : "r"(0x41));
    __asm__ volatile("ecall");
    char c;
    __asm__ volatile("mv %0, a0" : "=r"(c));
    return c;
}

void putc(char c)
{
    __asm__ volatile("mv a1, %0" : : "r"((uint64)c));
    __asm__ volatile("mv a0, %0" : : "r"(0x42));
    __asm__ volatile("ecall");
}