//
// Created by os on 6/24/22.
//

#include "../h/MemoryAllocator.hpp"
#include "../test/printing.hpp"

MemoryAllocator &MemoryAllocator::getInstance() {
    static MemoryAllocator myInstance;
    return myInstance;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    // obezbediti da size > maxSize -> size = maxSize;
    size *= MEM_BLOCK_SIZE;

    // try to find an existing free block in the list(first fit algorythm)
    BlockHeader *blk = MemoryAllocator::getInstance().freeMemHead, *prev = nullptr;
    while(blk) {
        if (blk->size >= size) break;
        prev = blk;
        blk = blk->next;
    }

    if (!blk) return nullptr;

    size_t remainingSize = blk->size - size;
    if (remainingSize >= sizeof(BlockHeader)) {
        // a fragment remains
        blk->size = size;
        size_t offset = sizeof(BlockHeader) + size;
        BlockHeader* newBlk = (BlockHeader*)((char*)blk+offset);
        if (prev) {
            prev->next = newBlk;
        }else {
            MemoryAllocator::getInstance().freeMemHead = newBlk;
        }
        newBlk->next = blk->next;
        newBlk->size = remainingSize - sizeof(BlockHeader);
    }else {
        // no remaining fragment, allocate the entire block
        if (prev) {
            prev->next = blk->next;
        } else {
            MemoryAllocator::getInstance().freeMemHead = blk->next;
        }
    }
    blk->next = nullptr;
    return (char*)blk + sizeof(BlockHeader);
}

void MemoryAllocator::printList() {
    printString("------Free Segment Lista------");
    MemoryAllocator& memoryAllocator = MemoryAllocator::getInstance();
    if(!memoryAllocator.freeMemHead)
        printString("LISTA JE PRAZNA!");
    for(BlockHeader * curr = memoryAllocator.freeMemHead; curr; curr = curr->next){
        printString("Adresa pocetka slobodnog bloka: ");
        printInt((uint64)curr);
        printString("Velicina slobodnog bloka: ");
        printInt((uint64)curr->size);
    }
    printString("------------------------------");
}

int MemoryAllocator::mem_free(void *address) {
    if (address == nullptr) return -1;
    BlockHeader* bh = (BlockHeader*)((char*)address - sizeof(BlockHeader));
    BlockHeader* tmp = nullptr;
    MemoryAllocator& ma = MemoryAllocator::getInstance();
    if (!ma.freeMemHead || (char*)bh < (char*)(ma.freeMemHead)) {
        tmp = nullptr;
    }else {
        tmp = ma.freeMemHead;
        while (tmp->next != nullptr && (char*)bh > (char*)(tmp->next)) {
            tmp = tmp->next;
        }
    }

    // insert the new segment after the tmp
    BlockHeader* newBlk = (BlockHeader*)bh;
    newBlk->size = bh->size;
    if (tmp) {
        newBlk->next = tmp->next;
    }else {
        newBlk->next = ma.freeMemHead;
    }
    if (tmp) {
        tmp->next = newBlk;
    }else {
        ma.freeMemHead = newBlk;
    }

    int ret1 = tryToJoin(newBlk);
    int ret2 = tryToJoin(tmp);

    int ret = 0;
    if (ret1 == -1 && ret2 == -1) {
        ret = -1;
    }
    return ret;
}

int MemoryAllocator::tryToJoin(BlockHeader *curr) {
    if (!curr) return -1;
    if (curr->next && (char*)curr + curr->size == (char*)(curr->next) - sizeof(BlockHeader)) {
        // remove the curr->next segment
        curr->size += curr->next->size + sizeof(BlockHeader);
        curr->next = curr->next->next;
        return 1;
    }
    return 0;
}