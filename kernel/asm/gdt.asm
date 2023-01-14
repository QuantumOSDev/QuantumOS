; GDT Loader

section .text
	global load_gdt
	global tss_flush

tss_flush:
	mov ax, 0x2B
	ltr ax
	ret

load_gdt:
	mov eax, [esp + 4]

	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	cli

	mov eax, cr0
	
	or eax, 1

	mov cr0, eax

	jmp 0x08:far_jump

far_jump:
	ret