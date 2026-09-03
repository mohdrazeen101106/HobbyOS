#include "kernel/panic/diagnostics.h"
#include "kernel/log/log.h"
#include "arch/x86/cpu/cpu.h"

static void _decode_page_fault(uint32_t error_code);
static void _decode_selector_error(uint32_t error_code);

static void _decode_page_fault(uint32_t error_code) {
    uint32_t cr2 = cpu_read_cr2();
    klog(LOG_FATAL, "Faulting address (CR2)=0x%08x", cr2);
    klog(LOG_FATAL, "Cause=%s Access=%s Mode=%s", 
        (error_code & 0x1u) ? "Protection-Violation" : "Page-Not-Present",
        (error_code & 0x2u) ? "Write" : "Read",
        (error_code & 0x4u) ? "User" : "Supervisor"
    );
    if(error_code & 0x8u) klog(LOG_FATAL, "Reserved-Bit-Violation");
    if(error_code & 0x10u) klog(LOG_FATAL, "Instruction-Fetch");
}

static void _decode_selector_error(uint32_t error_code) {
    if(!error_code) {
        klog(LOG_FATAL, "Error code is zero, not selector-related");
        return;
    }

    bool ext = error_code & 0x1u;
    bool from_idt = error_code & 0x2u;
    bool from_ldt = error_code & 0x4u;
    uint32_t sel_idx = (error_code >> 3) & 0x1FFFu;

    klog(LOG_FATAL, "SelectorIndex=%u Table=%s External=%s", sel_idx, from_idt ? "IDT" : (from_ldt ? "LDT" : "GDT"), ext ? "YES" : "NO");
}

static void panic_dump_exception_details(const exception_descriptor_t* desc, const interrupt_frame_t* frame) {
    if(!desc || !desc->has_error_code) return;

    switch (frame->vector)
    {
    case EXCEPTION_VECTOR_PAGE_FAULT:
        _decode_page_fault(frame->error_code);
        break;
    case EXCEPTION_VECTOR_INVALID_TSS:
    case EXCEPTION_VECTOR_GENERAL_PROTECTION:
    case EXCEPTION_VECTOR_SEGMENT_NOT_PRESENT:
    case EXCEPTION_VECTOR_STACK_SEGMENT_FAULT:
        _decode_selector_error(frame->error_code);
        break;
    default:
        break;
    }
}

static void panic_dump_registers(const interrupt_frame_t* frame) {
    klog(LOG_FATAL, "Vector=%u ErrorCode=0x%08x", frame->vector, frame->error_code);
    klog(LOG_FATAL, "EAX=0x%08x EBX=0x%08x ECX=0x%08x EDX=0x%08x", frame->eax, frame->ebx, frame->ecx, frame->edx);
    klog(LOG_FATAL, "ESI=0x%08x EDI=0x%08x EBP=0x%08x ESP=0x%08x", frame->esi, frame->edi, frame->ebp, frame->esp);
    klog(LOG_FATAL, "EIP=0x%08x CS=0x%08x EFLAGS=0x%08x", frame->eip, frame->cs, frame->eflags);
    klog(LOG_FATAL, "DS=0x%08x ES=0x%08x FS=0x%08x GS=0x%08x", frame->ds, frame->es, frame->fs, frame->gs);
}

void panic_report(const exception_descriptor_t* desc, const interrupt_frame_t* frame) {
    klog(LOG_FATAL, "KERNEL PANIC");

    if(frame) panic_dump_registers(frame);
    if(desc) {
        klog(LOG_FATAL, desc->name);
        klog(LOG_FATAL, desc->description);
        if(frame) panic_dump_exception_details(desc, frame);
    }
    else klog(LOG_FATAL, "Unknown CPU Exception");

    klog(LOG_FATAL, "System Halted.");
}