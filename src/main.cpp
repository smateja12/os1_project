//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"

extern void userMain(void*);

int main()
{
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    TCB* mainThread = TCB::createThread(nullptr, nullptr, nullptr);
    TCB::running = mainThread;
    TCB::thread_start(mainThread);
    printString("Main started!\n");

    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    /*
    thread_t korThread;
    thread_create(&korThread, userMain, nullptr);

//    thread_init(&korThread, userMain, nullptr);
//    thread_start(&korThread);

    while(!korThread->isFgFinished()) {
        thread_dispatch();
    }
    */

    userMain(nullptr);

    printString("Main finished!\n");

//    delete korThread;
    delete TCB::running;

    return 0;
}
