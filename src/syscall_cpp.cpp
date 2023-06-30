//
// Created by os on 8/8/22.
//

#include "../h/syscall_cpp.h"

Thread::Thread(void (*body)(void *), void *arg)
{
//    thread_create(&myHandle, (TCB::Body)body, (void*)arg);
    thread_init(&myHandle, (TCB::Body)body, (void*)arg);
}

Thread::~Thread()
{
    delete myHandle;
}

void Thread::dispatch()
{
    thread_dispatch();
}

Thread::Thread()
{
    thread_init(&myHandle, threadRunWrapper, this);
//    thread_create(&myHandle, threadRunWrapper, this);
}

int Thread::start()
{
    int tmp = thread_start(&myHandle);
    return tmp;
}

void Thread::threadRunWrapper(void *arg)
{
    if (arg)
    {
        Thread* t = (Thread*)(arg);
        t->run();
    }
}

