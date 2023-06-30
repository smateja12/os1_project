//
// Created by marko on 20.4.22..
//

#include "../lib/mem.h"

#include "../h/MemoryAllocator.hpp"

#include "../h/syscall_c.h"

using size_t = decltype(sizeof(0));

void *operator new(size_t n)
{
    /*
    // Moj MemoryAllocator
    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    return ptr;
    */
//    return __mem_alloc(n);
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    /*
    // MOJ MemoryAllocator
    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    return ptr;
    */
//    return __mem_alloc(n);
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    /*
    // MOJ MemoryAllocator
    MemoryAllocator::getInstance().mem_free(p);
    */
     //    __mem_free(p);
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    /*
    // MOJ MemoryAllocator
    MemoryAllocator::getInstance().mem_free(p);
    */
     //    __mem_free(p);
    mem_free(p);
}