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

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
  {
    printf("%s", Mix_GetError());
  }

  map_t *m;
  int finished=0;

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


  Mix_Music *musique; //Création du pointeur de type Mix_Music
  musique = Mix_LoadMUS("data/Oops.mp3"); //Chargement de la musique
  Mix_PlayChannel(1, musique, 0);
  while (!finished) {


    finished=getEvent(m);

    if(tour > 200) {
      m->temps++;
      if(m->temps == 0)
        printf("1\n");
      update(m);

      if(m->getBonus == -1) {
        switch (checkBonus(m)) {
          case 0 :
            m->voiture.vitesse = m->voiture.vitesse*2; break;
          case 2 :
            printf("Play song\n"); break;
          default: break;
        }
      }
        


      if(carArriveInDestination(m) == 1) {
        tour=0;
        m->temps=0;
        m->level++;
        initGame(m);

        Mix_Pause(1);
      }

      int secondes = (getNext() - m->temps_1)/ 1000;
      if (secondes > TEMPS_MAX) {
        //exit(0);
      }

    } else if (m->level == 0 && tour < 200) {
      if(Mix_Paused(1) == 1) //Si le canal 2 est en pause
      {
        Mix_Resume(1); //Le canal 2 peut maintenant rejouer
      }

       m->temps_1 = getNext();

    }

    if (tour < 200) {
      Mix_Resume(-1);
    }
    timerWait();

    paint(r,m,m->temps);

    m->temps = (tour > 200) ? m->temps + 1 : 0;
    tour++;
  }

  Mix_FreeMusic(musique);
  Mix_CloseAudio(); //Fermeture de l'API
  SDL_Quit();

  return 0;
}
