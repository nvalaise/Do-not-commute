#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "engine.h"
#include "graphics.h"
#include "timer.h"
#define MAP "data/map.bmp"

int main(int argc,char *argv[]) {
  map_t *m;
  int finished=0;
  m=loadMap(MAP);
  SDL_Renderer *r = openWindow(800,600); /* A changer ! */
  loadTiles(r);
  timerInit();
  while (!finished) {
    finished=getEvent(m);
    update(m);
    paint(r,m);
    timerWait();
    }
  return 0;
}
