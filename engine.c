#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "engine.h"

#define   M_PI   3.14159265358979323846

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



  if (getpixel(m->background,center_x,center_y) == 0xcea ) { // herbe
    m->voiture.x += cos(rad) * m->voiture.vitesse / 2;
    m->voiture.y += sin(rad) * m->voiture.vitesse / 2;

  } else { // route
    m->voiture.x += cos(rad) * m->voiture.vitesse;
    m->voiture.y += sin(rad) * m->voiture.vitesse; 

  }

  int test_x = abs((int) fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(rad) * m->voiture.largeur)/2 ); // + cos(rad) + (1-sin(rad))
  int test_y = abs((int) fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2); //+ sin(rad) + (1-cos(rad))

  if (getpixel(m->background,test_x,test_y) == 0xfed) { // mur
    m->voiture.x = origine_x;
    m->voiture.y = origine_y; 

  }


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

void initGame(map_t *m) {
  int i,j;
  int compteur = 0;
  for (i=0; i < m->largeur; i++) {
    for (j=0; j < m->hauteur; j++) {
      compteur = getpixel(m->background,i,j) == 0x00f ? compteur + 1 : compteur;
    }
  }

  m->checkpoints = (int*)malloc(sizeof(int*)*compteur);

  for(i=0; i < compteur; i++) {
    m->checkpoints[i] = (int*) malloc(sizeof(int)*2);
  }
  
  compteur = 0;
  for (i=0; i < m->largeur; i++) {
    for (j=0; j < m->hauteur; j++) {
      if(getpixel(m->background,i,j) == 0x00f) {
        m->checkpoints[compteur][0] = i;
        m->checkpoints[compteur][1] = j;
        compteur=compteur+1;
      }
    }
  }

  srand(time(NULL));
  m->rang_checkpoints_src = rand() % compteur;

  do {
    m->rang_checkpoints_dest = rand() % compteur;
  } while(m->rang_checkpoints_src == m->rang_checkpoints_dest);


  m->voiture.x = m->checkpoints[m->rang_checkpoints_src][0] - 20;
  m->voiture.y = m->checkpoints[m->rang_checkpoints_src][1] - 20;

}

int carArriveInDestination(map_t *m) {
  SDL_Rect rect_dest;
  rect_dest.x = m->checkpoints[m->rang_checkpoints_dest][0]-20;
  rect_dest.y = m->checkpoints[m->rang_checkpoints_dest][1]-20;
  rect_dest.h = 40;
  rect_dest.w = 40;


  int center_x = fabsf((int) m->voiture.x + m->voiture.largeur/2);
  int center_y = fabsf((int) m->voiture.y + m->voiture.hauteur/2);
  
  SDL_Point car_center;
  car_center.x = center_x;
  car_center.y = center_y;

  if(SDL_PointInRect(&car_center, &rect_dest)) {
    return 1;
  }

  return 0;
}

