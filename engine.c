#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "engine.h"

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
        m->voiture.angle -= 12;
        break;
      case SDLK_RIGHT:  
        m->voiture.angle += 12;
        break;
      default: ;
      }
    } 
  }
  return 0;
} 

/* A COMPLETER */
void update(map_t *m) {
  int width, height;
  width = m->largeur-m->voiture.largeur;
  height = m->hauteur-m->voiture.hauteur;

  float origine_x, origine_y;
  origine_x = m->voiture.x;
  origine_y = m->voiture.y;

  //printf("origine %f - %f\n", origine_x, origine_y);

  int center_x = fabsf((int) m->voiture.x + m->voiture.largeur/2);
  int center_y = fabsf((int) m->voiture.y + m->voiture.hauteur/2);

  double angle = m->voiture.angle-90;
  double rad = angle * M_PI / 180.0;
  //printf("rad %lf\n", rad);



  if (getpixel(m->background,center_x,center_y) == 0xcea ) {
    m->voiture.x += cos(rad) * m->voiture.vitesse / 2;
    m->voiture.y += sin(rad) * m->voiture.vitesse / 2;

    //printf("herbe\n");
  } else {
    m->voiture.x += cos(rad) * m->voiture.vitesse;
    m->voiture.y += sin(rad) * m->voiture.vitesse; 

    //printf("route\n");

  }

  int test_x = abs((int) fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(rad) * m->voiture.largeur)/2 ); // + cos(rad) + (1-sin(rad))
  int test_y = abs((int) fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2); //+ sin(rad) + (1-cos(rad))

  printf("x:%d - y: %d\n", test_x, test_y);

  if (getpixel(m->background,test_x,test_y) == 0xfed) {
    m->voiture.x = origine_x;
    m->voiture.y = origine_y; 

    //exit(0);

    //printf("mur\n");

  }



  //printf("%04x\n",getpixel(m->background,center_x,center_y));




  if (m->voiture.x <= 0){
    m->voiture.x = 0;
  }
  if (m->voiture.x >= width){
    m->voiture.x = width;
  }
  if (m->voiture.y <= 0){
    m->voiture.y = 0;
  }
  if (m->voiture.y >= height){
    m->voiture.y = height;
  }
}
