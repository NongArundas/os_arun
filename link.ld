ENTRY(loader)                /* Tell the linker that 'loader' is the starting point */

SECTIONS {
    . = 0x00100000;          /* Load the code at exactly 1 Megabyte into the RAM */

    .text ALIGN (0x1000) :   /* Put all executable code here */
    {
        *(.text)
    }

    .rodata ALIGN (0x1000) : /* Put all read-only data (like text strings) here */
    {
        *(.rodata)
    }

    .data ALIGN (0x1000) :   /* Put initialized variables here */
    {
        *(.data)
    }

    .bss ALIGN (0x1000) :    /* Put uninitialized variables here */
    {
        *(.bss)
    }
}
