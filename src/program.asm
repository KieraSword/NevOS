ORG 589552 ; &progbuf
BITS 32 ; nasm compiles as 16-bit by default and causes a #UD
section .data
msg db "Hello world from userspace!", 10
len equ $ - msg
section .text
_start:
	mov eax, 5 ; sys_write
	mov ebx, 1 ; stdout
	mov ecx, msg
	mov edx, len
	int 0x42
	mov eax, 1 ; sys_exit
	mov ebx, 0
	int 0x42
