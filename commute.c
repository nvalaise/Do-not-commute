#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "graphics.h"
#include "timer.h"

#define MAP "data/map0.bmp"
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

/*
#define MAP "data/map2.bmp"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1280
*/

int main(int argc,char *argv[]) {
  map_t *m;
  int finished=0;
  m=loadMap(MAP, WINDOW_WIDTH,WINDOW_HEIGHT);
  SDL_Renderer *r = openWindow(WINDOW_WIDTH,WINDOW_HEIGHT); /* A changer ! */
  loadTiles(r,m);
  timerInit();
  while (!finished) {
    finished=getEvent(m);
    update(m);
    paint(r,m);
    timerWait();
    }
  return 0;
}
