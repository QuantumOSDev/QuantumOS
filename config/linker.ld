/*
*  link.ld
*/
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
   . = 0x100000;
   .text 0x0100000 : {
      __kernel_text_section_start = .;
      . = ALIGN(4)

      LONG(0x1BADB002)
      LONG(0x00000003)
      LONG(0xE4524FFB)

      code = .; _code = .; __code = .;

      *(.text)
      *(.rodata)

      . = ALIGN(4096)

      __kernel_text_section_end = .;
   }

   .data : {
      __kernel_data_section_start = .;

      data = .; _data = .; __data = .;

      *(.data)

      . = ALIGN(4096)

      __kernel_data_section_end = .;
   }

   .rodata : {
      __kernel_rodata_section_start = .;

      *(.rodata)

      __kernel_rodata_section_end = .;
   }

   .bss  : {
      __kernel_bss_section_start = .;

      bss = .; _bss = .; __bss = .;

      *(.bss)

      . = ALIGN(4096)

      __kernel_bss_section_end = .;
   }

   end = .; _end = .; __ end = .;

   __kernel_section_end = .;
}
