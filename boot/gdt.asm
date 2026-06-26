gdt_start:

gdt_null:			;The mandatory GDT null descriptor
	dd 0x0			;Double word, i.e. 4 bytes defined to 0
	dd 0x0			;In total, 8 bytes are defined to 0 and
					;forms the null descriptor

gdt_code:			;The code segment descriptor
					; Base=0x0000, limit=0xfffff,
					; 1st flags: (present) 1 (privilege) 00 (decriptor type) 1 -> 1001b
					; type flags: (code) 1 (conforming) 0 (readable) 1 (accessed) 0 -> 1010b
					; 2nd flags: (granularity) 1 (32-bit default) 1 (64-bit seg) 0 (AVL) 0 -> 1100b
	dw 0xffff		;Limit (bits 0-15)
	dw 0x0			;Base (bits 0-15)
	db 0x0			;Base (bits 16-23)
	db 10011010b	;1st flags, type flags
	db 11001111b	;2nd flags, Limit (bits 16-19)
	db 0x0			;Base (bits 24-31)

gdt_data:			;The data segment descriptor
					; Same base, limit, 1st and 2nd flags, except for the type flags
					; type flags: (code) 0 (expand down) 0 (writable) 1 (accessed) 0 -> 0010b
	dw 0xffff		;Limit (bits 0-15)
	dw 0x0			;Base (bits 0-15)
	db 0x0			;Base(bits 16-23)
	db 10010010b	;1st flags, type flags
	db 11001111b	;2nd flags, Limit (bits 16-19)
	db 0x0			;Base (bits 24-31)

gdt_end:

gdt_descriptor:					;GDT descriptor
	dw gdt_end - gdt_start - 1 	; Size of our GDT, always one less than the true size
	dd gdt_start				; Start address of our GDT (32-bit address!)

; Offsets contained in segment registers in protected mode
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
