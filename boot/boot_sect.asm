[org 0x7c00]
[bits 16]
KERNEL_OFFSET equ 0x1000		; For now Kernel Offset is fixed at 0x1000
								; Might be subject to future change

mov [BOOT_DRIVE], dl			; BIOS moves the boot drive to dl on start up, save this

; Set up the stack
mov bp, 0x9000
mov sp, bp

; Print greeting messages, mainly for debug
mov bx, MSG_REAL_MODE
call print_string

call load_kernel				; Load the kernel to memory
call switch_to_pm				; Make the switch to 32-bit protected
								; mode

jmp $

load_kernel:
	; Debug message
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET		; Load the kernel at the defined offset
	mov dh, 34					; Loading a hardcoded number of sectors
	mov dl, [BOOT_DRIVE]		; TODO - Switch to dynamically loading number of sectors
	call disk_load				; either by some macro or by implementing a file-system aware
								; bootloader

	ret

%include "print_string.asm"		; 16-bit mode print routine
%include "disk_load.asm"		; sector loading routine from a floppy disk
%include "gdt.asm"				; Global Descriptor Table loading routine for segmentation
%include "print_string_pm.asm"	; 32-bit mode print routine
%include "switch_to_pm.asm"		; Routine to make a far jump and switch to protected mode

[bits 32]

BEGIN_PM:
	; Debug message
	mov ebx, MSG_PROT_MODE
	call print_string_pm

	call KERNEL_OFFSET	; Call the entry point defined at the start of kernel offset
						; Stored in kernel_entry.asm

	jmp $

; Global Variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0x0d, 0x0a, 0
MSG_LOAD_KERNEL db "Loading Kernel now!", 0x0d, 0x0a, 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55