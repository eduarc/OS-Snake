/* UTILS.C
 * eduarcastrillo@gmail.com
 */
#include "string.h"
#include "io.h"
#include "utils.h"

uint32 seed = 0x50000;	// Semilla para generacion de numero pseudo-aleatorio

unsigned int time() { 
	// IMPLEMENTACION INCOMPLETA...
	unsigned int secs = 0;
	int wait = 0;

	__asm__("cli");

	do {
		outb(0x70, 0xA);
		wait = inb(0x71) & 0x80;
	} while(wait); //Esperar a que CMOS no este en modo Update

	outb(0x70, 0x0); // Registro para Segundos
	secs += inb(0x71);

	outb(0x70, 0x2); // Registro para Minutos
	secs += inb(0x71) * 60;

	outb(0x70, 0x4); // Registro para Horas
	secs += inb(0x71) * 3600;
	
	outb(0x70, 0x9); // Registro para Anio
	secs += inb(0x71) * 365 * 24 * 60 * 60;

	__asm__("sti");

	return secs;
}

/* Formula generadora de numeros pseudo-aleatorios tomada de 
 * http://www.15seconds.com/issue/051110.htm
 */
int rand() {

	seed = (seed * 0x43FD43FD + 0xC39EC3) % 0x1000000;
	return seed;
}

void srand(uint32 rseed) {

	int8 *pseed = (int8 *)&rseed;
	int tmp = ((pseed[0] ^ pseed[2]) << 8) | (pseed[1] ^ pseed[3]);
	if((int32)seed < 0) seed = tmp ^ 0x06C0;
	else seed = tmp ^ 0x0240;
}

int isdigit(int c) {

	return c >= 0x30 && c <= 0x39;
}

int isxdigit(int c) {

	return isdigit(c)
			|| (c>= 0x41 && c <= 0x46)
				|| (c>= 0x61 && c <= 0x66);
}

int xatoi(const char *str) {	// Prefix '0x'

	int v = 0, fix;
	for(str += 2; *str; ++str) {
		fix = 0x30;
		if(*str > 0x40) fix = 0x37;
		v <<= 4;
		v |= *str - fix;
	}
	return v;
}

int atoi(const char *str) {

	int n = 0;
	int sig = 1;
	if(*str == '+') ++str;
	else if(*str == '-') {
		sig = -1;
		++str;
	}
	for(;*str;++str) {
		n *= 10;
		n += *str - 0x30;
	}
	return n*sig;
}

void parsex(char *buff, unsigned hex) {

    *buff++ = '0';
    *buff++ = 'x';
	if(hex == 0) {
		*buff++ = '0';
		*buff = '\0';
		return;
	}
	int offset = 6;
	int x = (hex >> 28) & 0xF;
	
	while(x == 0) {		// Eliminar ceros a la izquierda
		x = (hex >> 4 * offset) & 0xF;
		--offset;
	}
	++offset;
	while(offset >= 0) {
		x = (hex >> 4 * offset) & 0xF;
		*buff = x + 0x37;
		if(x < 0xA) {
			*buff = x + 0x30;
		}
		++buff;
		--offset;
	}
    *buff = '\0';
}

void parsei(char *buff, int dec) {
	int i = 0, v;
	while(dec) {
		v = dec % 10;
		v += 0x30;
		buff[i] = v;
		dec /= 10;
		++i;
	}
	buff[i--] = '\0';

	int k = (buff[0] == '-') ? 1 : 0;
	int tmp;
	for(;i < k; ++k, --i) {
		tmp = buff[i];
		buff[i] = buff[k];
		buff[k] = tmp;
	}
}

