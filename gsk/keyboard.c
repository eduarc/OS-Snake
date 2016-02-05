/* KEYBOARD.C
 * eduarcastrillo@gmail.com
 */
#include "common.h"
#include "io.h"
#include "isr.h"
#include "keyboard.h"

static volatile int waiting_hit = 0;
static kbint_callback int_callback = 0;

static void irq_kb(registers_t regs) {

	if(waiting_hit) {	// Esperando dato desde kbhit()
		waiting_hit = 0;
		return;
	}
	uint8 data = inb(0x60);
	if(int_callback) int_callback(data);
	while(inb(0x64) & 2); // esperar a que se procese el dato
}	

void setup_keyboard(kbint_callback callback) {

	int_callback = callback;
	register_interrupt_handler(IRQ1, irq_kb);		// registrar ISR para el teclado
}

int kbhit() {

	waiting_hit = 1;
	while(waiting_hit);
	uint8 sc = inb(0x60);
	return sc;
}
