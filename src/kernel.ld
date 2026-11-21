ENTRY(kmain)
SECTIONS {
 . = 0;
 .text : { KEEP(*(.text.entry)) *(.text*) }
 .rodata : { *(.rodata*) }
 .data : { *(.data*) }
 .bss : { *(.bss*) *(COMMON) }
 /DISCARD/ : { *(.eh_frame*) *(.comment*) *(.note*) }
}
