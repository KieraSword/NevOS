[BITS 16]
[ORG 0x7C00]
jmp boot
gdt_start:
gdt_null:
	dq 0x0000000000000000
	
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
    
gdt_user_code:
    dw 0xFFFF
    dw 0
    db 0
    db 11111010b
    db 11001111b
    db 0

gdt_user_data:
    dw 0xFFFF
    dw 0
    db 0
    db 11110010b
    db 11001111b
    db 0

gdt_tss:
    dw 0x0067
    dw 0
    db 0
    db 10001001b
    db 0
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_offset equ gdt_start - $$
boot:
	mov ax, 0x0003
    int 0x10
    cli

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x500
    
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    mov ah, 0x02
    mov al, 32
    mov ch, 0
    mov cl, 2
    mov dh, 0
	int 0x13

	mov ah, 0x88
	int 0x15
	add ax, 1024
	
	mov ax, 0x2401
	int 0x15
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp 0x08:flush

[BITS 32]
flush:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
	jmp 0x10000
	
times 510 - ($ - $$) db 0
dw 0xAA55
