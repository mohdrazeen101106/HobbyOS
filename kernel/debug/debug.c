#include "debug.h"

void debug_trigger_invalid_opcode(void)
{
    __asm__ volatile ("ud2");
}