/*
    CPU module that contains inline assembly for essential cpu tasks, so that this is the only
    inline assembly interface for the rest of the kernel.
*/
#include "cpu.h"

void cpu_cli(void) {
    __asm__ volatile ("cli" : : : "memory");
}

void cpu_sti(void) {
    __asm__ volatile ("sti" : : : "memory");
}

void cpu_hlt(void) {
    __asm__ volatile ("hlt" : : : "memory");
}

void cpu_pause(void) {
    __asm__ volatile ("pause");
}

uint32_t cpu_read_eflags(void) {
    uint32_t result;
    __asm__ volatile (
        "pushfl\n\t"
        "popl %0"
        : "=r" (result)
    );
    return result;
}

uint32_t cpu_read_cr0(void) {
    uint32_t result;
    __asm__ volatile ("mov %%cr0, %0" : "=r" (result));
    return result;
}

uint32_t cpu_read_cr2(void) {
    uint32_t result;
    __asm__ volatile ("mov %%cr2, %0" : "=r" (result));
    return result;
}

uint32_t cpu_read_cr3(void) {
    uint32_t result;
    __asm__ volatile ("mov %%cr3, %0" : "=r" (result));
    return result;
}

void cpu_write_cr3(uint32_t value) {
    __asm__ volatile ("mov %0, %%cr3" : : "r" (value) : "memory");
} 

void cpu_invlpg(void *addr) {
    __asm__ volatile ("invlpg (%0)" : : "r" (addr) : "memory");
}