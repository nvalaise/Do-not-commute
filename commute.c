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
  loadCheckpoints(m);
  initGame(m);
  SDL_Renderer *r = openWindow(m->largeur,m->hauteur);
  loadTiles(r,m);

  timerInit();

  m->temps = 0;
  int tour = 0;

  while (!finished) {

    finished=getEvent(m);


    if(tour > 200) {
      update(m);
      m->temps++;

      if(carArriveInDestination(m) == 1) {
        tour=0;
        m->temps=0;
        m->level++;
        initGame(m);
      }
      
      int secondes = (getNext() - m->temps_1)/ 1000;
      if (secondes > TEMPS_MAX) {
        exit(0);
      }

    } else if (m->level == 0 && tour < 200) {
      m->temps_1 = getNext();
    }
    timerWait();

    paint(r,m,m->temps);

    tour++;
  }
  return 0;
}
