/*
 * Funciones para el manejo basico de la pantalla
 * en modo texto 80x25x16
 * eduarcastrillo@gmail.com
 */
#ifndef _SCREEN_H
#define _SCREEN_H

#include "common.h"

// Retorna el valor del atributo formado por las componentes Background y Foreground
#define ATTRIB(bg, fg) (bg << 4) | (fg & 0xF)

// 16 Colores
enum {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE
};

/* funciones para manejo de doble buffer */
void double_buffer(int __db);
void swap_buffers();

/* Funciones para impresion de caracteres en pantalla */
inline void putchar(char __c);
inline void attr_putchar(char __c, uint8 __attr);
inline void putcharxy(char __c, int __x, int __y);
void attr_putcharxy(char __c, int __x, int __y, uint8 __attr);

/* Imprime un Null-termintated string en pantalla */
inline void prints(const char *__str);
void attr_prints(const char *__str, uint8 __attr);

/* Funciones para el control de la pantalla */
void vscroll(int __n);		// Scroll vertical hacia arriba N veces
void clrscr();				// Borrar el contenido de la pantalla
void background(uint8 __color);
void foreground(uint8 __color);

/* Funciones varias para el cursor */
void show_cursor();
void hide_cursor();
/* Funciones para el posicionamiento del cursor */
inline void gotox(int __x);
inline void gotoy(int __y);
void gotoxy(int __x, int __y);
/* Funciones para localizacion del cursor en pantalla */
int getx();
int gety();
void getxy(int *__x, int *__y);

#endif /* _SCREEN_H */
