#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "engine.h"

#define   M_PI   3.14159265358979323846 /* pi */

/* Capture les evenements clavier/fenetre 
    Retourne 1 si il faut quitter le jeu, 0 sinon.
    A COMPLETER
*/
int getEvent(map_t *m) {
  SDL_Event event;
  /* Ecoute les événements qui sont arrivés */
  while( SDL_PollEvent(&event) ){
    /* On a fermé la fenetre -> quitter le jeu */
    if (event.type==SDL_QUIT) return 1;
    /* On a appuyé sur une touche */
    if (event.type==SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:  return 1;
      case SDLK_LEFT:    
        m->voiture.angle -= 15;
        break;
      case SDLK_RIGHT:  
        m->voiture.angle += 15;
        break;
      default: ;
      }
    } 
  }
  return 0;
} 

/* A COMPLETER */
void update(map_t *m) {
  double angle = m->voiture.angle-90;
  double rad = angle * M_PI / 180.0;
  m->voiture.x += cos(rad) * m->voiture.vitesse;
  if(m->voiture.y > 0)
    m->voiture.y += sin(rad) * m->voiture.vitesse;
  else {
    m->voiture.y = 1;
  }

  printf("y : %d\n", m->voiture.y);
}
