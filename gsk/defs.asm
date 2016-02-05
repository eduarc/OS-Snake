; I/O Ports y definiciones varias
; eduarcastrillo@gmail.com
bits 32

; Funcion para leer un byte desde el puerto
; especificado
global inb
inb:
	xor eax, eax
	mov dx, [esp+4]		; Puerto
	in al, dx
	ret

; Funcion para leer una palabra desde el puerto
; especificado
global inw
inw:
	xor eax, eax
	mov dx, [esp+4]		; Puerto
	in ax, dx
	ret

; Funcion para escrbir un byte desde el puerto
; especificado
global outb
outb:
	mov al, [esp+8]		; Puerto
	mov dx, [esp+4]		; Dato
	out dx, al
	ret 

; Funcion que registra la IDT en la CPU
global idt_flush
idt_flush:
    mov eax, [esp+4]
    lidt [eax]      
    ret
	
