#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "engine.h"
#include "graphics.h"
#include "timer.h"

#ifdef __APPLE__
#include <SDL2_mixer/SDL_mixer.h>
#elif __linux__
#include <SDL2/SDL_mixer.h>
#else
#   error "Unknown compiler"
#endif

#define MAP "data/mapFB.bmp"

//#define MAP "data/map0.bmp"
//#define MAP "data/map2.bmp"

int main(int argc,char *argv[]) {

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
    printf("%s", Mix_GetError());
  }

  map_t *m;
  int play=1;

  m=loadMap(MAP);
  
  loadCheckpoints(m);
  initGame(m);
  SDL_Renderer *r = openWindow(m->largeur,m->hauteur);
  loadTiles(r,m);
  loadFlammes(r);
  loadTilesLevel(r);
  loadTilesBonus(r);

  timerInit();

  m->temps = 0;
  int tour = 0;

  Mix_AllocateChannels(5);

  Mix_Chunk *son1, *son2, *son3, *son4;
  son1 = Mix_LoadWAV("data/FB.wav");
  son2 = Mix_LoadWAV("data/epic_song.wav");
  son3 = Mix_LoadWAV("data/klaxon.wav");
  son4 = Mix_LoadWAV("data/FB_generic.wav");

  Mix_VolumeChunk(son1, MIX_MAX_VOLUME);
  Mix_VolumeChunk(son2, MIX_MAX_VOLUME);
  Mix_VolumeChunk(son3, MIX_MAX_VOLUME);
  Mix_VolumeChunk(son4, MIX_MAX_VOLUME);

  Mix_PlayChannel(4, son4, 0);

  int old_state = 1;
  while (play) {


    play=getEvent(m);
    if(play != old_state) {
      switch (play) {
        case 1: break;
        case 2: 
          Mix_HaltChannel(4);
          Mix_PlayChannel(1, son1, -1);
          break;
      }
      old_state = play;
    }

    switch(m->type_menu) {
      case 1:
        paintMenu(r,m);
        break;
      case 2:
        if(tour > 200) {
          timerJeuxWait();

          m->temps++;
          //if(m->temps == 0)
            //printf("1\n");
          update(m);


          if(m->boolKlakson == 1) {
            Mix_PlayChannel(3, son3, 0);
            m->boolKlakson = 0;
          }
          if(m->getBonus == -1) {
            switch (checkBonus(m)) {
              case 0 :
                m->voiture.vitesse = m->voiture.vitesse*2; break;
              case 1 :
                m->voiture.vitesse++; break;
              case 2 :
                if (Mix_Playing(2) == 1) {
                  Mix_Pause(2);
                  Mix_PlayChannel(1, son1, -1);
                } else if (Mix_Playing(1) == 1) {
                  Mix_Pause(1);
                  Mix_PlayChannel(2, son2, -1);
                }
                break;
              default: break;
            }
          }
          if(carArriveInDestination(m) == 1) {
            tour=0;
            m->temps=0;
            m->level++;
            initGame(m);
          }

          int secondes = (getNext() - m->temps_1)/ 1000;
          if (secondes > TEMPS_MAX) {
            //exit(0);
          }

        } else if (m->level == 0 && tour < 200) {
           m->temps_1 = getNext();
        }

        paint(r,m);

        if (tour < 200) {

        }

        m->temps = (tour > 200) ? m->temps + 1 : 0;
        tour++;
        break;

      default:
        break;
    }

    timerWait();

  }

  Mix_FreeChunk(son1);
  Mix_FreeChunk(son2);

  Mix_CloseAudio(); //Fermeture de l'API
  SDL_Quit();

  return 0;
}
