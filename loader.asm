; Segundo estadio de carga y configuracion del bootloader
; eduarcastrillo@gmail.com
bits 32

global _start
_start:
	extern kmain
	call kmain
	jmp $
