//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

bool TCB::inSystemMode = true;

int TCB::sid = 0;

void TCB::changeWorkingMode()
{
    if (TCB::inSystemMode)
    {
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
        TCB::inSystemMode = false;
    }else
    {
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
        TCB::inSystemMode = true;
    }
}

TCB *TCB::createThread(Body body, void* arg, uint64* stack)
{
    return new TCB(body, arg, stack, TIME_SLICE);
}

void TCB::yield()
{
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    TCB *old = running;
    TCB::timeSliceCounter = 0; // resetovanje brojaca
    if (!old->isFgFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    // promena rezima rada
    TCB::changeWorkingMode();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFgFinished(true);
    thread_dispatch();
}

int TCB::thread_exit()
{
    if (!TCB::running) return -1;
    TCB::running->setFgFinished(true);
    TCB::dispatch();
    return 0;
}

int TCB::thread_start(TCB *tcb)
{
    int tmp = -1;
    if (!tcb) return tmp;
    if (tcb->isFgCreated())
    {
        if (tcb->getBody() != nullptr)
        {
            Scheduler::put(tcb);
            tmp = 0;
        }
    }
    return tmp;
}

int TCB::thread_init(TCB** tcb, TCB::Body body, void *arg, uint64* stack)
{
    *tcb = TCB::createThread(body, arg, stack);
    if (*tcb == nullptr) return -1;
    else return 0;
}

int TCB::thread_create(thread_t *handle, TCB::Body body, void *arg, uint64 *stack)
{
    *handle = TCB::createThread(body, arg, stack);
    int tmp = -1;
    if (*handle == nullptr) return tmp;
    else { tmp = TCB::thread_start(*handle); }
    return tmp;
}

void *TCB::operator new(TCB::size_t n)
{
//    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
//    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
//    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;
//
//    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
//    return ptr;
    return mem_alloc(n);
}

void *TCB::operator new[](TCB::size_t n)
{
//    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
//    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
//    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;
//
//    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
//    return ptr;
    return mem_alloc(n);
}

void TCB::operator delete(void *p) noexcept
{
//    MemoryAllocator::getInstance().mem_free(p);
    mem_free(p);
}

void TCB::operator delete[](void *p) noexcept
{
//    MemoryAllocator::getInstance().mem_free(p);
    mem_free(p);
}
