/* Kernel Main 
 * eduarcastrillo@gmail.com
 */
#include "snake.h"
#include "gsk.h"

/* KMain */
int kmain() {

  init_gsk();
  snake_videogame();
  //ping_pong_videogame();
  return 0;
}
