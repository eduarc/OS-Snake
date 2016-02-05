/* 
 * Algunas funciones de utileria
 * eduarcastrillo@gmail.com
 */
#ifndef _UTILS_H
#define _UTILS_H

#include "common.h"

/* Tiempo en segundos del reloj del CMOS, Real-Time Clock */
unsigned int time();

/* Generador de numeros pseudo-aleatorios */
int rand();
void srand(unsigned int __seed);

/* Caracteres */
int isdigit(int __c);
int isxdigit(int __c);

/* Conversion de cadenas a valores numericos */
int xatoi(const char *__str_hex);
int atoi(const char *__str_dec);

/* Conversion de valores numericos a cadenas */
void parsex(char *__buff, unsigned __hex);
void parsei(char *__buff, int __dec);

#endif /* _UTILS_H */
