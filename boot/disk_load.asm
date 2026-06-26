; Wrapper subroutine to load a specific number of sectors from the disk
; Implemented with floppy disks in view, implement for CDs and DVDs in the future
; Arguments:
; 	dh, the number of sectors to load
;	dl, the drive to load
; Only loads from cylinder 0 and head 0 as per implementation requirements
disk_load:
	push dx

	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02

	int 0x13
	jc disk_error

	pop dx
	cmp dh, al
	jne disk_error
	ret

disk_error:
	mov bx, ERROR_MSG
	call print_string
	jmp $

ERROR_MSG: db "Disk read error!", 0
