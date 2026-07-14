ENTRY(_start)

KERNEL_LOAD_ADDRESS = 0x1000;

PHDRS
{
    text PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = KERNEL_LOAD_ADDRESS;

    __kernel_start = .;

    .text ALIGN(16) :
    {
        __text_start = .;

        KEEP(*(.text.boot))

        *(.text)
        *(.text.*)

        __text_end = .;
    } :text

    .rodata ALIGN(16) :
    {
        __rodata_start = .;

        *(.rodata)
        *(.rodata.*)

        __rodata_end = .;
    } :text

    .data ALIGN(16) :
    {
        __data_start = .;

        *(.data)
        *(.data.*)

        __data_end = .;
    } :data

    __kernel_file_end = .;

    .bss ALIGN(16) (NOLOAD) :
    {
        __bss_start = .;

        *(COMMON)

        *(.bss)
        *(.bss.*)

        __bss_end = .;
    } :data

    __kernel_memory_end = .;

    /DISCARD/ :
    {
        *(.comment)
        *(.note.GNU-stack)
    }

    ASSERT(
        _start == KERNEL_LOAD_ADDRESS,
        "Kernel entry point is not at the load address"
    )

    ASSERT(
        __bss_end >= __bss_start,
        "Invalid BSS boundaries"
    )

    ASSERT(
        __kernel_memory_end > __kernel_start,
        "Kernel image is empty"
    )

    ASSERT(
        __kernel_memory_end <= 0x7C00,
        "Kernel overlaps the BIOS boot sector"
    )
}