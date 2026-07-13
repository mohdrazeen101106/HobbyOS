#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

/*
    struct idt_entry
    An entry into the Interrupt Descriptor Table(IDT), used to specify the interrupt
    handler for the specific interrupt
*/
typedef struct idt_entry
{
    uint16_t offset_low; // Lowest 2 bytes of the 32 bit address of the handler
    uint16_t segment_selector; // Segment in which the handler is present
    uint8_t zero; // H-L: 0, 0, 0, rest reserved
    uint8_t flags; // Flags(H-L): Present, DPL(2), 0, Gate Size, 1, 1, 0
    uint16_t offset_high; // Highest 2 bytes of the 32 bit address of the handler
} __attribute__((packed)) idt_entry;

/*

*/
typedef struct idt_descriptor
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_descriptor;

/*
    struct cpu_state
    Stores the register states right before the interrupt is triggered
    The order matters here, as edi is the last register to be pushed onto the stack
    while eax is the first register to be pushed
*/
typedef struct cpu_state {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} __attribute__((packed)) cpu_state;

/*
    struct interrupt_frame
    Stores the interrupt data right before the C handler is called
    Mainly has the error code and interrupt number
*/
typedef struct interrupt_frame {
    uint8_t int_no; // The interrupt number pushed by the macro
    uint32_t error_code; // The error code pushed by the macro
    uint32_t eip; // Pushed by CPU
    uint32_t cs; // Pushed by CPU
    uint32_t eflags; // Pushed by CPU
} __attribute__((packed)) interrupt_frame;

extern struct idt_entry idt[IDT_SIZE]; //Define the IDT
extern struct idt_descriptor idt_d; //Define the IDT descriptor used by lidt

void init_idt( void );
void interrupt_handler(struct cpu_state *cpu_s, struct interrupt_frame *int_frame);
void idt_set_gate(uint8_t int_no, uint32_t handler, uint16_t segment_selector, uint8_t flags);
extern void idt_load(struct idt_descriptor *idt_d);

#endif