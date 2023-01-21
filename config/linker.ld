ENTRY(_start)
 
SECTIONS
{
	__kernel_section_start = .;
	. = 1M;
 
	.text : {
		__kernel_text_section_start = .;
		*(.multiboot)
		*(.text)
        . = ALIGN(4096);
		__kernel_text_section_end = .;
	}
 
	.rodata : {
        __kernel_rodata_section_start = .;
        *(.rodata)
        __kernel_rodata_section_end = .;
    }

	.data : {
		__kernel_data_section_start = .;
		*(.data)
        . = ALIGN(4096);
        __kernel_data_section_end = .;
	}
 
	.bss : {
        __kernel_bss_section_start = .;
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
        __kernel_bss_section_end = .;
    }

    end = .; _end = .; __end = .;
    __kernel_section_end = .;
}
