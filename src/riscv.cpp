//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

#include "../h/MemoryAllocator.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    // dohvatanje prosledjenih argumenata
    uint64 volatile reg4;
    __asm__ volatile("ld %0, 112(fp)" : "=r"(reg4));
    uint64 volatile reg3;
    __asm__ volatile("ld %0, 104(fp)" : "=r"(reg3));
    uint64 volatile reg2;
    __asm__ volatile("ld %0, 96(fp)" : "=r"(reg2));
    uint64 volatile reg1;
    __asm__ volatile("ld %0, 88(fp)" : "=r"(reg1));
    uint64 volatile numOfSysCall;
    __asm__ volatile("ld %0, 80(fp)" : "=r"(numOfSysCall));

    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch (numOfSysCall) {
            case 0x01:
                {
                    // dohvatanje prosledjenih arugmenata
                    auto numOfBlocks = (size_t)(reg1);

                    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(ptr));
                }
                break;
            case 0x02:
                {
                    // dohvatanje prosledjenih arugmenata
                    void* ptr = (void*)(reg1);

                    int retval = MemoryAllocator::getInstance().mem_free(ptr);
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(retval));
                }
                break;
            case 0x11:
                // thread_create
                {
                    // dohvatanje prosledjenih arugmenata
                    TCB** handle = (TCB**)(reg4);
                    auto body = (TCB::Body)(reg3);
                    void* args = (void*)(reg2);
                    auto* stack = (uint64*)(reg1);

                    int retval = TCB::thread_create((TCB**)handle, (TCB::Body)body, (void*)args, (uint64*)stack);
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(retval));
                }
                break;
            case 0x12:
                // thread_exit
                {
                    int retval = TCB::thread_exit();
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(retval));
                }
                break;
            case 0x13:
                // thread_dispatch
                {
                    TCB::dispatch();
                }
                break;
            case 0x14:
                // thread_start
                {
                    // dohvatanje prosledjenih arugmenata
                    TCB** handle = (TCB**)(reg1);

                    int retval = TCB::thread_start(*handle);
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(retval));
                }
                break;
            case 0x15:
                // thread_init
                {
                    // dohvatanje prosledjenih arugmenata
                    TCB** handle = (TCB**)(reg4);
                    auto body = (TCB::Body)(reg3);
                    void* args = (void*)(reg2);
                    auto* stack = (uint64*)(reg1);

                    int retval = TCB::thread_init(handle, (TCB::Body)(body), (void*)(args), (uint64*)(stack));
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(retval));
                }
                break;
            case 0x41:
                // getc
                {
                    char c = __getc();
                    __asm__ volatile("sd %0, 80(fp)" : : "r"(c));
                }
                break;
            case 0x42:
                // putc
                {
                    // dohvatanje prosledjenih arugmenata
                    char c = (char)(reg1);

                    __putc(c);
                }
                break;
            default:
                break;
        }
        // IZMENJENO
//        TCB::timeSliceCounter = 0;
//        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        // IZMENJENO
//        TCB::timeSliceCounter++;
//        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
//        {
//            uint64 volatile sepc = r_sepc();
//            uint64 volatile sstatus = r_sstatus();
//            TCB::timeSliceCounter = 0;
//            TCB::dispatch();
//            w_sstatus(sstatus);
//            w_sepc(sepc);
//        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        printString("-----------\n");
        printString("SCAUSE: ");
        printInt(scause);
        printString("\n");
        printString("SEPC: "); // gde se desio prekid
        printInt(r_sepc());
        printString("\n");
        printString("STVAL: "); // dodatno objasnjenje interrupt-a
        printInt(r_stval());
        printString("\n-----------\n");
    }
}