/* 
 * Libreria que contiene Funciones para manipulacion
 * de Puertos de la computadora.
 * eduarcastrillo@gmail.com
 */
#ifndef _IO_H
#define _IO_H

#include "common.h"
/* 
 * Prototipos para las funciones I/O Ports 
 * Definidas en defs.asm
 */
extern int8  inb(int16 __port) _CDECL_;
extern int16 inw(int16 __port) _CDECL_;
extern void outb(int16 __port, int8 __data) _CDECL_;

#endif	/* _IO_H */

