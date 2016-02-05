/* TIMER.C
 * eduarcastrillo@gmail.com
 */
#include "io.h"
#include "isr.h"
#include "timer.h"

static volatile uint64 ticks = 0;	// Cuenta de reloj

static void irq_timer(registers_t regs) {
	++ticks;
}

void setup_timer(unsigned freq) {

	register_interrupt_handler(IRQ0, irq_timer);	// registrar IH para el timer

	uint16 val = 1193180 / freq;
	outb(0x43, 0x34);
	outb(0x40, val & 0xFF);
	outb(0x40, val >> 8);
}

uint64 tick_count() {

	return ticks;
}

void delay(uint64 total_ticks) {

	uint64 limit = ticks + total_ticks;
	while(ticks <= limit);
}
