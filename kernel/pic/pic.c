#include "pic.h"
#include "../io/io.h"
#include <stdint.h>

/*
    void pic_remap
    Remap the PIC interrupt vector offsets
    Initially they are mapped to 0x0 and 0x8, which clash with CPU 
    interrupts. Remapping these offsets requires a special initialization process
    where each PIC chip expects three or four words - ICW1, ICW2, ICW3 (and ICW4)
*/
void pic_remap() {
    // ICW1: Initialize
    // 0x10 is the initialization command for that chip
    // 0x01 tells that we are also going to input ICW4
    port_byte_out(PIC1_COMMAND, 0x11);
    port_byte_out(PIC2_COMMAND, 0x11);

    // ICW2: Set vector offsets
    // Set vector offset for master PIC chip to 32
    // Set vector offset for slave PIC chip to 32+8 = 40
    port_byte_out(PIC1_DATA, 0x20);
    port_byte_out(PIC2_DATA, 0x28);

    // ICW3: Connecting master and slave chips
    // Tell the master PIC chip at which IRQ the slave is connected
    // 0x04 = 0b00000100, the second bit is set hence IRQ2 connects to slave chip
    port_byte_out(PIC1_DATA, 0x04);
    // Tell the slave PIC chip it is connected to IRQ2 of the master chip
    // This is not a bitmask for some reason
    port_byte_out(PIC2_DATA, 0x02);

    // ICW4: Set operation flags
    // Set the 1st bit i.e., set the operating mode to 8086/8088 mode
    // There are other flags for buffered mode, auto end-of-interrupt but we set
    // only this for now
    port_byte_out(PIC1_DATA, 0x01);
    port_byte_out(PIC2_DATA, 0x01);

    // Now we can mask the specific interrupts
    // Bit set = That specific interrupt is masked and disabled
    // Bit cleared = That specific interrupt is enabled
    // Only unmask the keyboard interrupt, i.e. PIC1 for now
    pic_disable();
    irq_enable(1);
}

/*
    void pic_disable
    Mask all the interrupts, required when switching to APIC or any other reason.
    Set the Internal Mask Register (IMR) of each PIC to 0xff.
    1 means that specific IRQ at the bit position is masked and disabled
    0 means taht specific IRQ at the bit position is unmasked and enabled
*/
void pic_disable() {
    port_byte_out(PIC1_DATA, 0xFF);
    port_byte_out(PIC2_DATA, 0xFF);
}

/*
    uint16_t pic_get_mask
    Return the contents of the Internal Mask Register (IMR) of the two PIC chips combined
    The higher 8 bits contain the Master IMR contents while the lower 8 bits contain the Slave
    IMR contents.
    The IMR of a PIC is read by reading the data port of the specific PIC
*/
uint16_t pic_get_mask() {
    return (port_byte_in(PIC1_DATA) << 8) | (port_byte_in(PIC2_DATA));
}

/*
    void pic_send_eoi
    Function that sends the End-Of-Interrupt signal to the required PIC.
*/
void pic_send_eoi(uint8_t IRQ_line) {
    if(IRQ_line > 7)
        port_byte_out(PIC2_COMMAND, PIC_EOI);  // Send to slave if required
    port_byte_out(PIC1_COMMAND, PIC_EOI);      // Always send to master
}

/*
    void irq_enable
    Unmask the specified IRQ line. The function expects the IRQ number and not a bitmask
*/
void irq_enable(uint8_t IRQ_line) {
    uint16_t port;
    
    if(IRQ_line < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQ_line -= 8;
    }

    // Clear the required bit
    port_byte_out(port, (uint8_t)(port_byte_in(port) & ~(1 << IRQ_line)));
}

/*
    void irq_disable
    Mask the specified IRQ line. The function expects the IRQ number and not a bitmask
*/
void irq_disable(uint8_t IRQ_line) {
    uint16_t port;

    if(IRQ_line < 8) port = PIC1_DATA;
    else {
        port = PIC2_DATA;
        IRQ_line -= 8;
    }
    // Set the required bit
    port_byte_out(port, port_byte_in(port) | (1 << IRQ_line));
}

/* --TODO-- Implement functions to read the ISR and IRR from the two PIC chips */