//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

using thread_t = TCB*;

// Thread Control Block
class TCB
{
public:

    ~TCB() { delete[] stack; }

    bool isFgFinished() const { return fgFinished; }

    void setFgFinished(bool finished) { TCB::fgFinished = finished; }

    bool isFgCreated() const { return fgCreated; }

    void setFgCreated(bool created) { TCB::fgCreated = created; }

    bool isFgBlocked() const { return fgBlocked; }

    void setFgBlocked(bool blocked) { TCB::fgBlocked = blocked; }

    bool isFgRunnable() const { return fgRunnable; }

    void setFgRunnable(bool runnable) { TCB::fgRunnable = runnable; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    Body getBody() const { return TCB::body; }

    static TCB *createThread(Body body, void* arg, uint64* stack);

    static void yield();

    static TCB *running;

    static bool inSystemMode;

    static int sid;

    int getId() const { return id; }

    static void dispatch();

    static int thread_exit();

    static int thread_start(TCB* tcb);

    static int thread_init(TCB** tcb, Body body, void* arg, uint64* stack);

    static int thread_create(thread_t* handle, Body body, void* arg, uint64* stack);

    // DODATO
    using size_t = decltype(sizeof(0));

    void *operator new(size_t n);
    void *operator new[](size_t n);
    void operator delete(void *p) noexcept;
    void operator delete[](void *p) noexcept;

private:

    TCB(Body body, void* args, uint64* s, uint64 timeSlice) :
            body(body),
            arg(args),
            stack(body != nullptr ? s : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            fgFinished(false),
            fgBlocked(false),
            fgRunnable(false)
    {
        // IZMENJENO
//        if (body != nullptr) { Scheduler::put(this); }
        this->fgCreated = true;
        this->id = sid++;
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool fgFinished, fgCreated, fgBlocked, fgRunnable;
    int id;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;
    static uint64 constexpr TIME_SLICE = 2;

    static void changeWorkingMode();

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
