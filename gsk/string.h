/*
 * Funciones para la manipulacion de memoria y strings
 * eduarcastrillo@gmail.com
 */
#ifndef _STRING_H
#define _STRING_H

void init_mem();

/* Funciones para manipulacion de memoria */
void *malloc(int __nbytes);
void free(void *__ptr);

void memcpy(void *__dest, void *__src, int n);
void memset(void *__dest, int __v, int __n);
void memsetw(void *__dest, int __v, int __n);

/* Funciones para manipulacion de strings */
void strcpy(char *__dest, const char *__src);
void strcat(char *dest, const char *src);
void substr(char *__dest, const char *__src, int __start_index, int __end_index);
int strlen(const char *__str);
int strncmp(const char *__str1, const char *__str2, int __n);

#endif /* _STRING_H */
