#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

void pic_remap(void); // Simple remap function
void pic_disable(void); // Mask all interrupts
uint16_t pic_get_mask(void); // Returns which IRQs are masked/unmasked
void pic_send_eoi(uint8_t IRQ_line); // Helper function to send pic EOI acknowledgement
void irq_enable(uint8_t IRQ_line); // Unmask the specific IRQ 
void irq_disable(uint8_t IRQ_line); // Mask the specific IRQ 

#endif