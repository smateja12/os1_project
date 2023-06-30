//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP

#include "../h/MemoryAllocator.hpp"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    // IZMENJENO
    Elem *head, *tail;

    using size_t = decltype(sizeof(0));

    void *operator new(size_t n);
    void *operator new[](size_t n);
    void operator delete(void *p) noexcept;
    void operator delete[](void *p) noexcept;
public:

    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }

};

template<typename T>
void *List<T>::operator new(List::size_t n) {
    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    return ptr;
}

template<typename T>
void *List<T>::operator new[](List::size_t n) {
    size_t maxNumOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - 16) / MEM_BLOCK_SIZE;
    size_t numOfBlocks = ((n - 1) / MEM_BLOCK_SIZE) + 1;
    if (numOfBlocks > maxNumOfBlocks) numOfBlocks = maxNumOfBlocks;

    void* ptr = MemoryAllocator::getInstance().mem_alloc(numOfBlocks);
    return ptr;
}

template<typename T>
void List<T>::operator delete(void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}

template<typename T>
void List<T>::operator delete[](void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
