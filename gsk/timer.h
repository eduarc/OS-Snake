/* 
 * Funciones para el manejo del Timer 
 * eduarcastrillo@gmail.com
 */
#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"

/* establece la fecuencia del PIT */
void setup_timer(unsigned __freq);
/* retorna el numero de ticks desde que se incia la computadora
 * La cuenta se realiza a la frecuencia actual del PIT
 */
uint64 tick_count();
/* realiza un retraso de N TICKS en la frecuencia actual */
void delay(uint64 __ticks);

#endif
