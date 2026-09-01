#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void cpu_cli(void);
void cpu_sti(void);
void cpu_hlt(void);
void cpu_pause(void);

uint32_t cpu_read_eflags(void);

uint32_t cpu_read_cr0(void);
uint32_t cpu_read_cr2(void);
uint32_t cpu_read_cr3(void);

void cpu_write_cr3(uint32_t value);

void cpu_invlpg(void *addr);

#endif