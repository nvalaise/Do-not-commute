#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "graphics.h"
#include "timer.h"

#define MAP "data/map0.bmp"
//#define MAP "data/map2.bmp"

int main(int argc,char *argv[]) {

  map_t *m;
  int finished=0;

  m=loadMap(MAP);
  printf("c\n");
  SDL_Renderer *r = openWindow(m->largeur,m->hauteur); /* A changer ! */
  printf("d\n");
  loadTiles(r,m);
  printf("e\n");
  timerInit();
  while (!finished) {
    finished=getEvent(m);
    update(m);
    paint(r,m);
    timerWait();
    }
  return 0;
}
