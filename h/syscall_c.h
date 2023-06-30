//
// Created by os on 9/2/22.
//

#ifndef PROJECT_BASE_7_SYSCALL_C_H
#define PROJECT_BASE_7_SYSCALL_C_H

#include "../h/tcb.hpp"

void* mem_alloc (size_t size);

int mem_free (void* ptr);

typedef TCB* thread_t;

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit();

void thread_dispatch();

int thread_start(thread_t* handle);

int thread_init(
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

const int EOF = -1;

char getc();

void putc(char c);

#endif //PROJECT_BASE_7_SYSCALL_C_H
