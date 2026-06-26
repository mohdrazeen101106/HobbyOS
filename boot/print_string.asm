print_string:
    pusha                   ; save all registers safely

print_string_loop:
    mov al, [bx]            ; load current character
    cmp al, 0               ; check for null terminator
    je print_string_done    ; exit if null
    mov ah, 0x0e            ; BIOS teletype function
    int 0x10                ; print character in al
    add bx, 1               ; advance to next character
    jmp print_string_loop
print_string_done:

    popa                    ; restore all registers
    ret
