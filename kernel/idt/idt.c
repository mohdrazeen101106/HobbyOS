#include "idt.h"
#include "../../drivers/screen/screen.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../pic/pic.h"

struct idt_entry idt[IDT_SIZE];
struct idt_descriptor idt_d;

/*
    void interrupt_handler
    The common C interrupt handler for all interrupts!
    Still actual functionality for each interrupt has to be separately implemented and 
    can be called from this function
*/
void interrupt_handler(struct cpu_state *cpu_s, struct interrupt_frame *int_frame) {
    // Get the interrupt number and error code from stack state
    uint8_t int_no = int_frame->int_no;
    // uint32_t error_code = int_frame->error_code;

    cpu_s->eax = 0; // Access CPU state as required, for now it isnt required

    switch (int_no)
    {
    case 33:
        keyboard_handler();
        break;
    
    default:
        break;
    }

    // Send EOI signal for hardware IRQs (32-47)
    if(int_no >= 32 && int_no <= 47) pic_send_eoi(int_no - 32);

    // Call the appropriate interrupts
    // Add interrupt functions here as needed by implementation!
    return;
}

/*
    void init_idt
    Initialize and load the IDT into memory
*/
void init_idt() {
    extern void *isr_table[256];

    for(uint8_t i = 1; i != 0; i++)
        idt_set_gate(i-1, (uint32_t)isr_table[i-1], 0x08, 0x8E);

    idt_d.limit = sizeof(idt) - 1;
    idt_d.base = (uint32_t)idt;

    idt_load(&idt_d);
}

/*
    void idt_set_gate
    Add an entry to the IDT, split the offset bytes and set the flags and segment selector
    accordingly
*/
void idt_set_gate(uint8_t int_no, uint32_t handler, uint16_t segment_selector, uint8_t flags) {
    idt[int_no].flags = flags;
    idt[int_no].segment_selector = segment_selector;
    idt[int_no].zero = 0;
    idt[int_no].offset_low = handler & 0xFFFF;
    idt[int_no].offset_high = (uint16_t)((handler >> 16) & 0xFFFF);
}