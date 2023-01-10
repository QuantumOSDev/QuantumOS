;Load the Global Descriptor Table

global kgdt_load_gdt
global kgdt_load_registers

kgdt_load_gdt:
	lgdt [esp + 4]
	ret

kgdt_load_registers:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	cli
	
	mov eax, cr0

	or eax, 1

	mov cr0, eax

	jmp 0x08:flush_cs

flush_cs:
	ret
