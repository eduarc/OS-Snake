/* SNAKE.C
 * eduarcastrillo@gmail.com
 */
#include "gsk.h"
#include "snake.h"

#define DIR_UP		0xA
#define DIR_LEFT	0xB
#define DIR_RIGHT	0xC
#define DIR_DOWN	0xD

#define NWALLS 8

char str_menu[] = 	"             XXXXX     XX   XX           XXX      XX    XX  XXXXXXXXX\n"
 					"           XXXX  XXX   XX XXXXXX       XXX XXX    XX   XX   XX\n"
					"          XXX      XX  XXXXX  XXXX    XXX   XXX   XX  XX    XX\n"
					"          XXXX         XXX      XXX  XXX     XXX  XXXXX     XX\n"
					"            XXXXXXX    XX        XX  XXXXXXXXXXX  XXXX      XXXXXXXXX\n"
					"               XXXXXX  XX        XX  XXXXXXXXXXX  XXXXX     XX\n"
					"          XX      XXX  XX        XX  XX       XX  XX  XX    XX\n"
					"           XXX  XXXX   XX        XX  XX       XX  XX   XX   XX\n"
					"             XXXXX     XX        XX  XX       XX  XX    XX  XXXXXXXXX\n"
					"\n"
			    	"                              Press ENTER to Play\n"
					"                            -----------------------\n"
			    	"                              Press ESC to Reboot\n";

int playing;
char str_points[20];
int points = 0;
int vel_delay = 50;

typedef struct __snake_node {
  int eating; 				    // si est'a comiendo galleta
  int x, y;					    // posicion en el map
  struct __snake_node *prev;
  struct __snake_node *next;
} snake_node_t;

typedef struct __snake_queue {
  struct __snake_node *head;	    	// Cabeza
  struct __snake_node *queue;	    	// Cola :)
} snake_queue_t;

typedef struct __wall {
  int x, y;
  int width, height;
} wall_t;

typedef struct __cookie {
  int x, y;
} cookie_t;

// Cuerpo del Snake inicial. 6 partes
snake_node_t baby_snake[6];
// SNAKE
snake_queue_t *snake;
int snake_mov_dir;
int pending_move = 0;
// Muros
wall_t walls[8];
// Unica galleta
cookie_t cookie;

/*
 FUNCIONES DE INICIALIZACION
 */
void create_snake() {

  snake->head = &baby_snake[0];
  snake_mov_dir = DIR_RIGHT;

  int x = 10;
  snake_node_t *q = snake->head;
  q->x = x--;
  q->y = 12;
  q->prev = 0;
  q->next = (q + 1);
  ++q;

  int i = 1;
  for(;i < 5; ++i, ++q) {
    q->x = x--;
    q->y = 12;
    q->prev = (q - 1);
    q->next = (q + 1);
  }
  q->x = x--;
  q->y = 12;
  q->prev = (q - 1);
  q->next = 0; 	// Cola
  snake->queue = q;
}

void create_wall(wall_t *w_t, int x, int y, int w, int h) {

	w_t->x = x;
	w_t->y = y;
	w_t->width  = w;
	w_t->height = h;
}

void create_map() {
		// Muros
	create_wall(&walls[0], 0, 0, 20, 1);
	create_wall(&walls[1], 0, 1, 2, 7);
	create_wall(&walls[2], 60, 0, 20, 1);
	create_wall(&walls[3], 78, 1, 2, 7);
	create_wall(&walls[4], 0, 23, 20, 1);
	create_wall(&walls[5], 0, 16, 2, 7);
	create_wall(&walls[6], 60, 23, 20, 1);
	create_wall(&walls[7], 78, 16, 2, 7);
		// Galleta
	cookie.x = 46;
	cookie.y = 12;
}

/*
 FUNCIONES DE DIBUJADO
 */
void draw_wall(wall_t *w) {

	uint8 attr = ATTRIB(BROWN, BLACK);
	int x, y = w->height - 1;
	for(; y >= 0; --y) {
		x = w->width - 1;
		for(; x >= 0; --x) {
			attr_putcharxy(' ', w->x + x, w->y + y, attr);
		}
	}
}

void draw_snake() {
	
	uint8 attr = ATTRIB(GREEN, BROWN);
	uint8 ex_attr = ATTRIB(LIGHT_GREEN, BROWN);
	snake_node_t *q = snake->head;

		// Pintar Cabeza
	if(q->eating) {
		attr_putcharxy('#', q->x, q->y, ex_attr);
	} else {
		attr_putcharxy('X', q->x, q->y, attr);
	}
		// Pintar Cola
	while(q) {
		if(q->eating) {
			attr_putcharxy('#', q->x, q->y, ex_attr);
		} else {
			attr_putcharxy('*', q->x, q->y, attr);
		}
		q = q->next;
	}
}

void draw_scene() {
	// WALLS
	int i = NWALLS - 1;
	for(; i >= 0; --i) {
		draw_wall(&walls[i]);
	}
	// COOKIE
	attr_putcharxy('@', cookie.x, cookie.y, ATTRIB(BLACK, RED));
	// SNAKE
	draw_snake();
	// PUNTOS
	parsei(str_points, points);
	gotoxy(5, 24);
	prints("POINTS: ");
	prints(str_points);
}

/*
 FUNCIONES MISCELANEAS Y ANIMACION
 */
int intercepts_wall(int x, int y) {
	
	wall_t *w;
	int i = NWALLS - 1;
	for(; i >= 0; --i) {
		w = &walls[i];
		if(x >= w->x && x <= w->width + w->x - 1
				&& y >= w->y && y <= w->height + w->y - 1)
		{
			return 1;
		}
	}
	return 0;
}

int intercepts_queue(snake_node_t *q, int x, int y) {

	while(q) {
		if(q->x == x && q->y == y) return 1;
		q = q->next;
	}
	return 0;
}

void animate() {

	snake_node_t *head  = snake->head;
	snake_node_t *queue = snake->queue;
	snake_node_t *new_head = queue;	// Nueva cabeza

		// ACTUALIZAR LA COLA
	if(queue->eating) {
		new_head = malloc(sizeof(snake_node_t));
		queue->eating = 0;
	} else {
		snake->queue = queue->prev;
		snake->queue->next = 0;
	}
		// MOVER CABEZA DEL SNAKE
	memcpy(new_head, head, sizeof(snake_node_t));
	switch(snake_mov_dir) {	// Mover Cabeza
		case DIR_UP: 	new_head->y--; 	break;
		case DIR_LEFT: 	new_head->x--; 	break;
		case DIR_RIGHT: new_head->x++; 	break;
		case DIR_DOWN: 	new_head->y++; 	break;
	}
	head->prev = new_head;
	new_head->next = head;
	snake->head = new_head;
	head = new_head;

	int hx = head->x;
	int hy = head->y;
		// VERIFICAR SI LLEGA AL LIMITE DEL MAPA
	if(hx > 79) {
		head->x = 0;
		hx = 0;
	}
	else if(hx < 0) {
		head->x = 79;
		hx = 79;
	}
	else if(hy > 23) {
		head->y = 0;
		hy = 0;
	}
	else if(hy < 0) {
		head->y = 23;
		hy = 23;
	}

	// VERIFICAR SI MUERE
	if(intercepts_queue(head->next, hx, hy)
			|| intercepts_wall(hx, hy)) {
		playing = 0;	// GAME OVER!! xD
		return;
	}
	int cx = cookie.x;
	int cy = cookie.y;
		// VERIFICAR SI COME GALLETA
	if(hx == cx && hy == cy) {
		head->eating = 1;
		points += 10;
		if(points % 100 == 0) vel_delay -= 2;
		do {	// Reubicar galleta
			cx = rand() % 80;
			cy = rand() % 24;
		} while(intercepts_queue(head, cx, cy) 
					|| intercepts_wall(cx, cy));
		cookie.x = cx;
		cookie.y = cy;
	} else {
		head->eating = 0;
	}
	pending_move = 0;
}

/*
 FUNCIONES DE CONTROL
 */
void kb_handler(uint8 scancode) {

	if(pending_move) return;

	switch(scancode) {
		case 0x48:	// KEY UP
		case 0xC8:
			if(snake_mov_dir != DIR_DOWN) {
				snake_mov_dir = DIR_UP;
				pending_move = 1;
			}
		break;
		case 0x50:	// KEY DOWN
		case 0xD0:
			if(snake_mov_dir != DIR_UP) {
				snake_mov_dir = DIR_DOWN;
				pending_move = 1;
			}    
		break;
		case 0x4B:	// KEY LEFT
		case 0xCB:
			if(snake_mov_dir != DIR_RIGHT) {
				snake_mov_dir = DIR_LEFT;
				pending_move = 1;
			}    
		break;
		case 0x4D:	// KEY RIGHT
		case 0xCD:
			if(snake_mov_dir != DIR_LEFT) {
				snake_mov_dir = DIR_RIGHT;
				pending_move = 1;
			}
		break;
		case 0x1:   // KEY ESC
			playing = 0;
		break;
	}
}

int menu() {
	
	clrscr();
	gotoxy(0, 5);
	prints(str_menu);
	int k;
	do {
		k = kbhit();
	} while(k != 0x1 && k != 0x1C);

	return k;
}

void play() {

	double_buffer(1);	// Activar doble buffer
	create_snake();
	create_map();
	vel_delay = 15;
	points = 0;
	playing = 1;

	while(playing) {
		draw_scene();
		swap_buffers();
		delay(vel_delay);
		animate();
	}
	double_buffer(0);
	gotoxy(2, 24);
	prints("GAME OVER!!! Press any key to continue...");
	kbhit();
}

void snake_videogame() {

	setup_timer(100);
	setup_keyboard(kb_handler);
	srand(time());
	hide_cursor();

	while(menu() != 0x1) play();
	outb(0x64, 0xFE);	// Reiniciar
}
