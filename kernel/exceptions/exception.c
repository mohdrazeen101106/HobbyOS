#include "exception.h"

static const exception_descriptor_t exception_table[32] = {
    [0] = {
        0,
        "Divide Error",
        "Division by zero or quotient overflow.",
        EXCEPTION_FAULT,
        false,
        false
    },

    [1] = {
        1,
        "Debug",
        "Debug Exception",
        EXCEPTION_FAULT,
        false,
        true
    },

    [2] = {
        2,
        "Non-Maskable Interrupt",
        "Non-Maskable hardware interrupt.",
        EXCEPTION_TRAP,
        false,
        false
    },

    [3] = {
        3,
        "Breakpoint",
        "Breakpoint instruction executed.",
        EXCEPTION_TRAP,
        false,
        true
    },

    [4] = {
        4,
        "Overflow",
        "INT0 instruction detected overflow",
        EXCEPTION_TRAP,
        false,
        true
    },

    [5] = {
        5,
        "Bound Range Exceeded",
        "BOUND instruction range exceeded",
        EXCEPTION_FAULT,
        false,
        false
    },

    [6] = {
        6,
        "Invalid Opcode",
        "Tried to execute an unknown or invalid opcode",
        EXCEPTION_FAULT,
        false,
        false
    },

    [7] = {
        7,
        "Device Not Available",
        "Math coprocessor unavailable.",
        EXCEPTION_FAULT,
        false,
        true
    },

    [8] = {
        8,
        "Double Fault",
        "Another exception occurred while handling another.",
        EXCEPTION_ABORT,
        true,
        false
    },

    [9] = {
        9,
        "Coproccessor Segment Overrun",
        "Legacy x87 segment overrun.",
        EXCEPTION_FAULT,
        false,
        false
    },

    [10] = {
        10,
        "Invalid TSS",
        "Task State Segment is invalid.",
        EXCEPTION_FAULT,
        true,
        false
    },

    [11] = {
        11,
        "Segment Not Present",
        "Referenced segment is not present.",
        EXCEPTION_FAULT,
        true,
        false
    },

    [12] = {
        12,
        "Stack Segment Fault",
        "Stack segment access fault",
        EXCEPTION_FAULT,
        true,
        false
    },

    [13] = {
        13,
        "General Protection Fault",
        "Protection violation.",
        EXCEPTION_FAULT,
        true,
        false
    },

    [14] = {
        14,
        "Page Fault",
        "Paging hardware failed to translate memory access.",
        EXCEPTION_FAULT,
        true,
        false
    },

    [15] = {
        15,
        "Reserved",
        "Reserved by intel",
        EXCEPTION_FAULT,
        false,
        false
    },

    [16] = {
        16,
        "x87 Floating Point",
        "Floating point exception",
        EXCEPTION_FAULT,
        false,
        true
    },

    [17] = {
        17,
        "Alignment Check",
        "Unaligned memory access",
        EXCEPTION_FAULT,
        true,
        false
    },

    [18] = {
        18,
        "Machine Check",
        "Fatal processor hardware error.",
        EXCEPTION_ABORT,
        false,
        false
    },

    [19] = {
        19,
        "SIMD Floating Point",
        "SIMD floating point exception.",
        EXCEPTION_FAULT,
        false,
        true
    },

    [20] = {
        20,
        "Virtualization",
        "Virtualization exception.",
        EXCEPTION_FAULT,
        false,
        false
    },

    [21] = {
        21,
        "Control Protection",
        "Control-flow Enforcement Technology violation.",
        EXCEPTION_FAULT,
        true,
        false
    },

    [22] = {22, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [23] = {23, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [24] = {24, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [25] = {25, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [26] = {26, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [27] = {27, "Hypervisor Injection", "Hypervisor Injection Exception.", EXCEPTION_FAULT, false, false},
    [28] = {28, "VMM Communication", "VMM communication exception.", EXCEPTION_FAULT, false, false},
    [29] = {29, "Security", "Security exception.", EXCEPTION_FAULT, true, false},
    [30] = {30, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
    [31] = {31, "Reserved", "Reserved by Intel.", EXCEPTION_FAULT, false, false},
};

const exception_descriptor_t* exception_get_descriptor(uint32_t vector) {
    if(vector >= 32) return 0;
    return &exception_table[vector];
}

const char* exception_get_name(uint32_t vector) {
    const exception_descriptor_t *desc = exception_get_descriptor(vector);

    if(desc == 0) return "Unknown";
    return desc->name;
}

const char* exception_get_description(uint32_t vector) {
    const exception_descriptor_t *desc = exception_get_descriptor(vector);
    
    if(desc == 0) return "Unknown Exception";
    return desc->description;
}

bool exception_has_error_code(uint32_t vector) {
    const exception_descriptor_t *desc = exception_get_descriptor(vector);

    if(desc == 0) return false;
    return desc->has_error_code;
}