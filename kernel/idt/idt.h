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
    struct interrupt_frame_t
    Stores the interrupt data, along with cpu register states right before the C handler is called
    Contains the complete context required for services like exception handling, task switching, etc.
*/
typedef struct interrupt_frame_t
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;

    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t ds;
    uint32_t es;
    uint32_t fs;
    uint32_t gs;

    uint32_t vector;
    uint32_t error_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} __attribute__((packed)) interrupt_frame_t;

extern struct idt_entry idt[IDT_SIZE]; //Define the IDT
extern struct idt_descriptor idt_d; //Define the IDT descriptor used by lidt

void init_idt( void );
void idt_set_gate(uint8_t int_no, uint32_t handler, uint16_t segment_selector, uint8_t flags);
extern void idt_load(struct idt_descriptor *idt_d);

#endif