[bits 32]

section .text.boot

global _start

[extern kmain]
[extern __bss_start]
[extern __bss_end]

_start:
	cld

	; Zero out the entire bss section
	mov edi, __bss_start
	mov ecx, __bss_end

	sub ecx, edi

	xor eax, eax

	rep stosb

	call kmain

.hang:
	cli
	hlt
	jmp .hang

SECTION .note.GNU-stack noalloc noexec nowrite progbits