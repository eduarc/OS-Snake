/* SCREEN.C
 * eduarcastrillo@gmail.com
 */
#include "string.h"
#include "io.h"
#include "screen.h"

#define OFFSET(x, y) (y * 80 + x)
#define PARSE(c, attr) ((attr << 8) | c)

static uint16  *vmem  = (uint16 *)0xB8000;
static uint64  *vmem64 = (uint64 *)0xB8000;
static int xpos = 0;
static int ypos = 0;
static uint8  attribute = 0x07;
static int dbuffer = 0;
static uint64 blank64;

static uint16 dbuff[80*25]; // Buffer para Doble Buffer
static uint64 *dbuff64 = (uint64 *)dbuff;

void double_buffer(int db) {

	dbuffer = db;
}

void swap_buffers() {

	uint64 b64 = blank64;
	int i = 20*25 - 1;
	for(; i >= 0;) {
		vmem64[i]  = dbuff64[i];
		dbuff64[i] = b64;
		--i;
		vmem64[i]  = dbuff64[i];
		dbuff64[i] = b64;
		--i;
		vmem64[i]  = dbuff64[i];
		dbuff64[i] = b64;
		--i;
		vmem64[i]  = dbuff64[i];
		dbuff64[i] = b64;
		--i;
		vmem64[i]  = dbuff64[i];
		dbuff64[i] = b64;
		--i;
	}
}

inline void putchar(char c) {

	attr_putcharxy(c, xpos, ypos, attribute);
}

inline void attr_putchar(char c, uint8 attr) {

	attr_putcharxy(c, xpos, ypos, attr);
}

inline void putcharxy(char c, int x, int y) {

	attr_putcharxy(c, x, y, attribute);
}

void attr_putcharxy(char c, int x, int y, uint8 attr) {

	if(dbuffer) {
		dbuff[OFFSET(x, y)] = PARSE(c, attr);
	} else {
		vmem[OFFSET(x, y)] = PARSE(c, attr);
	}
}

inline void prints(const char *str) {

	attr_prints(str, attribute);
}

void attr_prints(const char *str, uint8 attr) {

	while(*str) {
		if(ypos >= 25) {
			ypos = 24;
			vscroll(1);
		}
		if(*str == '\n' || xpos == 79) {
			xpos = 0;
			++ypos;
		}
		else if(*str == '\r') {
			xpos = 0;
		}
		else if(*str == '\t' && xpos % 4 == 0) {
			xpos += 4;
		} else {
			attr_putchar(*str, attr);
			++xpos;
		}
		gotoxy(xpos, ypos);
		++str;
	}
}

void vscroll(int n) {

	if(n == 0) return;
	int dx = 80*n;
	int dy = 25-n;
	memcpy(vmem, vmem + dx, dy*80*2);
	memsetw(vmem + dx, PARSE(' ', attribute), dy*40);
}

void clrscr() {
		// Funciona bien :)
	uint64 b64 = blank64;
	int i = 20*25 - 1;
    for (; i >= 0;) {
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
		vmem64[i--] = b64;
    }
}

void background(uint8 bgc) {

	attribute = (bgc << 4) | (attribute & 0xF);
		// valor utilizado en clrscr y swap_buffers
	blank64 = PARSE(' ', attribute);
	blank64 |= (blank64 << 48) | (blank64 << 32) | (blank64 << 16);
}

void foreground(uint8 fc) {

    attribute = (attribute & 0xF0) | (fc & 0xF);
}

void show_cursor() {

	outb(0x3D4, 0xA);
	outb(0x3D5, 0xE);
}

void hide_cursor() {

	outb(0x3D4, 0xA);
	outb(0x3D5, 0x1F);
}

inline void gotox(int x) {

	gotoxy(x, ypos);
}

inline void gotoy(int y) {

	gotoxy(xpos, y);
}

void gotoxy(int x, int y) {

	xpos = x;
	ypos = y;
	int offset = OFFSET(xpos, ypos);
    outb(0x3D4, 0xE);
    outb(0x3D5, offset >> 8);
	outb(0x3D4, 0xF);
    outb(0x3D5, offset);
}

int getx() {

	return xpos;
}

int gety() {

	return ypos;
}

void getxy(int *x, int *y) {

	*x = xpos;
	*y = ypos;
}
