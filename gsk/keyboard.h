/* 
 * Funciones para el manejo de teclado
 * eduarcastrillo@gmail.com
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

typedef void (*kbint_callback)(uint8);

/* registra la rutina encarga de manejar las pulsaciones de teclado */
void setup_keyboard(kbint_callback __callback);
/* espera por un golpe de teclado */
int kbhit();

/* completar con algunas otra funciones ... */

#endif /* _KEYBOARD_H */
