/* SOUND.C
 * eduarcastrillo@gmail.com
 * 
 * Falta probrar esto... :(
 * Al parecer no funciona en emuladores
 */
#include "io.h"
#include "sound.h"
#include "timer.h"

void nosound() {

	outb(0x61, inb(0x61) & 0xFC);
}

void play_sound(uint16 freq, uint16 del) {

	uint16 val = 1193180 / freq;
 	outb(0x43, 0xb6);
 	outb(0x42, val & 0xFF);
 	outb(0x42, val >> 8);

	uint8 ppi = inb(0x61);
	if(ppi != (ppi | 3)) outb(0x61, ppi | 3);

	delay(del);
	nosound();

		// Frecuencia original del PIT
	val = 1193180 / 100;
 	outb(0x43, 0x34);
 	outb(0x42, val & 0xFF);
 	outb(0x42, val >> 8);
}
