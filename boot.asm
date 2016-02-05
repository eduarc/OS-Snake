; Pequenio Bootloader
; Quise implementar uno, no utilizar GRUB
; eduarcastrillo@gmail.com
bits 16

	; Se utiliza el par seg:offset 0x0000:0x7C00
org 0x7C00

	cli 	; Deshabilitar interrupciones

	; Configurar los registros de segmentos DS, ES, SS
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

	; Cargar el kernel a la memoria (Carga maximo 18 Kbs de datos) Suficientes por ahora!!!
	; para mayores cantidades de datos, solo realizar repeticiones
	; leyendo las cabezas, cilindros,y sectores correspondientes
	; se lee desde Floppy Disk utilizando la BIOS
load_gsk:
	mov di, 0
_read:
		; Reiniciar el controlador del Floppy
	xor ax, ax
	int 0x13

	mov ah, 0x02 		; Funcion leer
	mov al, 0x24		; Leer 36 sectores
	mov dh, 0x00		; Head 0
	mov ch, 0x00		; Cylinder 0
	mov cl, 0x02		; Leer desde el segundo sector 
						; (El primer sector contiene el codigo del Bootloader)
	mov bx, 0x7E00		; El buffer de destino es ES:BX = 0x0000:0x7E00
						; Justo despues del codigo del Bootloader
	int 0x13			; Ejecutar interrupcion del Floppy
	jnc load_success	; Cargar correcta
	cmp di, 3
	je print_error		; Mostrar error de carga
	inc di
	jmp _read

load_success:
	; Pasar a modo protegido 32 bits
	lgdt [gdt_ptr]

	mov eax, cr0
	or al, 0x1
	mov cr0, eax
	
	mov ax, 0x10	; Segmento de datos
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov esp, 0x7BFF		; Puntero a la pila (32 Kbs disponibles)
						; Justo antes del codigo del Bootloader
	sti				; Habilitar interrupciones
	jmp 0x8:0x7E00	; Saltar al codigo del kernel

	; Imprimir un mensaje de error en caso de no poder
	; Cargar el kernel en memoria
print_error:
	mov si, error_msg
_next:
	lodsb
	or al, al
	jz halt_sys
	mov ah, 0xE
	int 0x10
	jmp _next

halt_sys:
	jmp $

; ----------------------------------------------
; GDT Para modo protegido
; Permitir acceso a 4GB de memoria en modo plano
; ----------------------------------------------
gdt:

null_entry:               ; Segmento nulo (En ocasiones no necesario), offset 0x0
        dq 0

code_entry:               ; Segmento de codigo, offset 0x8
        dw 0xFFFF		  ; limite  0  - 15
        dw 0			  ; base   16 - 31
        db 0			  ; base   32 - 39
        db 10011010b      ; byte de acceso  40 - 47
        db 11001111b	  ; limite  48 - 51 flags 52 - 55
        db 0			  ; base   56 - 63

data_entry:               ; Segmento de datos, offset 0x10
        dw 0xFFFF
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0
gdt_end:

gdt_ptr dw gdt_end - gdt - 1	; limit address
		dd gdt					; base address

error_msg db "Error when loading kernel", 0xD, 0xA, "System Halted...", 0x0

times 510-($-$$) db 0
dw 0xAA55
