#include <stdio.h>
#include <stdint.h>
#include <errno.h>

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void* result;

    __asm__ volatile (
        "movq %1, %%rdi\n"   // in rdi
        "movq %2, %%rsi\n"   // in rsi
        "movq %3, %%rdx\n"   // in rdx
        "movq %4, %%r10\n"   // flags in r10
        "movq %5, %%r8\n"    // in r8
        "movq %6, %%r9\n"    // offset in r9
        "movq %7, %%rax\n"   // syscall number (SYS_MMAP) in rax
        "syscall\n"          // trigger the syscall
        "movq %%rax, %0\n"   // from rax
        : "=r" (result)
        : "r" ((uint64_t)addr), "r" ((uint64_t)length), "r" ((uint64_t)prot),
          "r" ((uint64_t)flags), "r" ((uint64_t)fd), "r" ((uint64_t)offset),
          "r" ((uint64_t)SYS_MMAP)
        : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9"
    );

    if ((intptr_t)result < 0) {
        errno = -(intptr_t)result;
        return (void*)-1;
    }

    return result;
}

const int unmap(void* addr, size_t length) {
    long result;

    __asm__ volatile (
        "movq %1, %%rdi\n"   // addr in rdi
        "movq %2, %%rsi\n"   // length in rsi
        "movq %3, %%rax\n"   // syscall number (SYS_MUNMAP) in rax
        "syscall\n"          // trigger the syscall
        "movq %%rax, %0\n"   // rax
        : "=r" (result)
        : "r" ((uint64_t)addr), "r" ((uint64_t)length), "r" ((uint64_t)SYS_MUNMAP)
        : "rax", "rdi", "rsi"
    );

    if (result < 0) {
        errno = -(int)result;
        return -1;
    }

    return 0;
}
