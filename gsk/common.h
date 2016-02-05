/* 
 * Tipos y macros de uso comun
 * eduarcastrillo@gmail.com
 */
#ifndef _COMMON_H
#define _COMMON_H

/* Macros utiles para interfazar codigo C con ASM */
#define _CDECL_  __attribute__((cdecl))
#define _PACKED_ __attribute__((packed))

#define NULL (void *)0
/* 
 * Alias para variables enteras 
 * Identificador por numero de bits por variable
 * Longitud para varibles en un procesador x86 comun
 */
typedef long long 			int64;
typedef unsigned long long 	uint64;
typedef int 				int32;
typedef unsigned int 		uint32;
typedef short 				int16;
typedef unsigned short 		uint16;
typedef char 				int8;
typedef unsigned char 		uint8;

#endif  /* _COMMON_H */
