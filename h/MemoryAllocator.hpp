//
// Created by os on 6/24/22.
//

#ifndef PROJECT_BASE_4_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_4_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {
private:
    MemoryAllocator() {
        freeMemHead = (BlockHeader*)((char*)HEAP_START_ADDR);
        freeMemHead->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(BlockHeader);
    }

    typedef struct BlockHeader{
        struct BlockHeader* next;
        size_t size;
    }BlockHeader;

    BlockHeader* freeMemHead = nullptr;

public:

    MemoryAllocator(MemoryAllocator& other) = delete;
    void operator=(const MemoryAllocator&) = delete;

    static MemoryAllocator& getInstance();

    void* mem_alloc(size_t size);
    int mem_free(void* address);

    static void printList();
    int tryToJoin(BlockHeader* curr);
};

#endif //PROJECT_BASE_4_MEMORYALLOCATOR_HPP
