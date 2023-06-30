//
// Created by os on 8/8/22.
//

#ifndef PROJECT_BASE_5_SYSCALL_CPP_H
#define PROJECT_BASE_5_SYSCALL_CPP_H

#include "../h/syscall_c.h"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start();
    static void dispatch ();
    static int sleep (time_t);
    static void threadRunWrapper(void* arg);

    TCB *getMyHandle() const { return myHandle; }

protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};

#endif //PROJECT_BASE_5_SYSCALL_CPP_H
