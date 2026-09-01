[bits 32]
[extern interrupt_dispatch]

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0        ; push 0 as dummy error code
    push dword %1       ; push the interrupt number
    jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1       ; push the interrupt number (CPU already pushed error code)
    jmp common_interrupt_handler
%endmacro

; Common handler — all ISR stubs jump here
; Stack layout at this point (top to bottom):
;   [eip]       - pushed by CPU
;   [cs]        - pushed by CPU
;   [eflags]    - pushed by CPU
;   [err_code]  - pushed by macro (dummy 0) or CPU
;   [int_no]    - pushed by macro
common_interrupt_handler:
    ; Push segment registers to kernel for debugging
    push gs
    push fs
    push es
    push ds

    pushad

    mov ax, 0x10            ; Reload Data Segment
    mov ds, ax
    mov es, ax

    cld

    push esp                ; Push the pointer to int_frame struct

    call interrupt_dispatch  ; call the C interrupt handler

    add esp, 4              ; clean up the pointer to struct pushed              
    
    popad                   ; clean up the normal registers

    pop ds                  ; clean up the segment registers
    pop es
    pop fs
    pop gs
    
    add esp, 8              ; clean up int_no + err_code
    iret                    ; restore eip, cs, eflags (CPU-pushed)

; ISR definition macros

; Exceptions 0-7
%assign i 0
%rep 8
    no_error_code_interrupt_handler i
%assign i i+1
%endrep

error_code_interrupt_handler 8      ; Double Fault

no_error_code_interrupt_handler 9    ; Coprocessor Segment Overrun

error_code_interrupt_handler    10   ; Invalid TSS
error_code_interrupt_handler    11   ; Segment Not Present
error_code_interrupt_handler    12   ; Stack Segment Fault
error_code_interrupt_handler    13   ; General Protection Fault
error_code_interrupt_handler    14   ; Page Fault

no_error_code_interrupt_handler 15   ; Reserved
no_error_code_interrupt_handler 16   ; x87 Floating Point Exception

error_code_interrupt_handler 17     ; Alignment Check

; Exceptions 18-31
%assign i 18
%rep 14
    no_error_code_interrupt_handler i
%assign i i+1
%endrep

; IRQs 32-47
%assign i 32
%rep 16
    no_error_code_interrupt_handler i
%assign i i+1
%endrep

; User defined interrupts 48-255
%assign i 48
%rep 208
    no_error_code_interrupt_handler i
%assign i i+1
%endrep

section .rodata

; ISR Table declaration
global isr_table

isr_table:
%assign i 0
%rep 256
    dd interrupt_handler_%+i
%assign i i+1
%endrep

section .text

; IDT load routine
global idt_load

idt_load:
    mov eax, [esp + 4]  ; Pointer to IDT descriptor struct
    lidt [eax]
    sti                 ; Re-enable interrupts as they were disabled
                        ; for switching from 16-bit to 32-bit mode
    ret

SECTION .note.GNU-stack noalloc noexec nowrite progbits