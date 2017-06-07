#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "graphics.h"
#include "timer.h"

/* Toutes les tuiles du jeu */
SDL_Texture *tile[ALL];
SDL_Texture *tile_background;
/*  
   Doit etre avec le meme ordre que l'enum dans le .h 
*/
const char *tilenames[]={
  "data/ambulance.bmp",	
  "data/minitruck.bmp",
  "data/taxi.bmp",
  "data/sport.bmp",
  "data/minivan.bmp",
  "data/truck.bmp",
  "data/car.bmp",
  "data/police.bmp",
  "data/viper.bmp"
};
/* Lit un pixel d'une carte. A utiliser dans loadMap.
   Retourne la couleur RGB du pixel aux coordonnées x,y.
   DEJA ECRIT
*/
int getpixel(SDL_Surface *surface, int x, int y) {
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 r,g,b,*p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  Uint32 pixel;
  switch(bpp) {
  case 1: pixel= *p; break;
  case 2: pixel= *(Uint16 *)p; break;
  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) pixel= p[0] << 16 | p[1] << 8 | p[2];
    else pixel= p[0] | p[1] << 8 | p[2] << 16;
    break;
  case 4: pixel= *(Uint32 *)p; break;
  default: pixel= 0;   
  }
  //printf(">>%d %d\n",surface->pitch,surface->format->BytesPerPixel);
  SDL_GetRGB(pixel, surface->format, &r, &g, &b);
  r=r>>4;
  g=g>>4;
  b=b>>4;
  return (r<<8)+(g<<4)+b;
}
/* Charge toutes les tuiles du jeu
   DEJA ECRIT
 */
void loadTiles(SDL_Renderer *s, const map_t *m) {


  int i,j,k;
  SDL_SetRenderDrawColor(s, 0, 0, 0, 0);
  for (i=0; i<ALL; i++)  {
    SDL_Surface *loadedImage=SDL_LoadBMP(tilenames[i]);
    if (loadedImage !=NULL) {
      Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
      SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
      tile[i]=SDL_CreateTextureFromSurface(s, loadedImage );
      SDL_FreeSurface(loadedImage);
    } else fprintf(stderr,"Missing file %s:%s\n",tilenames[i],SDL_GetError());
  }

  tile_background = SDL_CreateTextureFromSurface(s, m->background);
}
/* Lecture d'une carte, comme MAP 
   A REMPLIR
*/
map_t *loadMap(char *filename) {
  
  map_t *m=(map_t*)malloc(sizeof(map_t));
  m->background = SDL_LoadBMP(filename);
  m->hauteur = m->background->h;
  m->largeur = m->background->w;
  m->level = 0;

  m->voiture.hauteur = 32;
  m->voiture.largeur = 32;

  m->voiture.vitesse = 2;
  m->voiture.angle = 0;
  //printf("%s\n", type_t[4]);

  return m;
}

/* Initialisation de la bibliotheque SDL, ouverture d'une fenetre de taille 
   w*SIZE x h*SIZE
 */
SDL_Renderer *openWindow(int w,int h) {
  SDL_Window *sdlWindow;
  SDL_Renderer *sdlRenderer;
  if (SDL_Init(SDL_INIT_VIDEO)<0) {
    fprintf(stderr,"Initialization error:%s\n",SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);
  if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, &sdlWindow, &sdlRenderer)) {
    fprintf(stderr,"Couldn't create window and renderer: %s",SDL_GetError());
    exit(1);
  }
  SDL_RenderSetLogicalSize(sdlRenderer, w, h);
  return sdlRenderer;
}
/* Redessine la carte, les joueurs, les effets, ... 
*/
void paint(SDL_Renderer *r,map_t *m, int t) {
  /* Fait un ecran noir */
  SDL_SetRenderDrawColor(r, 0, 0, 255, 255 );
  SDL_RenderClear(r);
  /* Definir ici le contenu graphique de la fenetre.
   */

  SDL_Rect rect_bg;
  rect_bg.x = 0;
  rect_bg.y = 10;
  rect_bg.h = m->hauteur;
  rect_bg.w = m->largeur;
  SDL_RenderCopy(r,tile_background,NULL,&rect_bg);


  SDL_SetRenderDrawColor(r, 255, 0, 0, 255 );

  SDL_Rect rect_progression_bg;
  rect_progression_bg.x = 0;
  rect_progression_bg.y = 0;
  rect_progression_bg.h = 10;
  rect_progression_bg.w = m->largeur;
  SDL_RenderFillRect(r, &rect_progression_bg);


  int secondes = (getNext() - m->temps_1)/ 1000;
  SDL_SetRenderDrawColor(r, 255, 255, 0, 255 );
  SDL_Rect rect_progression;
  rect_progression.x = 0;
  rect_progression.y = 0;
  rect_progression.h = 10;
  rect_progression.w = (secondes*m->largeur) / TEMPS_MAX;
  SDL_RenderFillRect(r, &rect_progression);

  SDL_SetRenderDrawColor(r, 255, 255, 0, 255 );
  SDL_Rect rect_src;
  rect_src.x = m->checkpoints[m->rang_checkpoints_src][0]-20;
  rect_src.y = m->checkpoints[m->rang_checkpoints_src][1]-20;
  rect_src.h = 40;
  rect_src.w = 40;
  SDL_RenderFillRect(r, &rect_src);

  SDL_SetRenderDrawColor(r, 0, 255, 0, 255 );
  SDL_Rect rect_dest;
  rect_dest.x = m->checkpoints[m->rang_checkpoints_dest][0]-20;
  rect_dest.y = m->checkpoints[m->rang_checkpoints_dest][1]-20;
  rect_dest.h = 40;
  rect_dest.w = 40;
  SDL_RenderFillRect(r, &rect_dest);

  SDL_Rect rect;
  rect.x = m->voiture.x;
  rect.y = m->voiture.y;
  rect.h = m->voiture.hauteur;
  rect.w = m->voiture.largeur;
  SDL_RenderCopyEx(r, tile[m->voiture.type], NULL, &rect, m->voiture.angle, NULL, SDL_FLIP_NONE);
  

  paintEnemies(r,m,t);


/*
  double angle = m->voiture.angle-90;
  double rad = angle * M_PI / 180.0;

  SDL_SetRenderDrawColor(r, 255, 0, 255, 255 );
  SDL_Rect car_x;
  car_x.x = fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
  car_x.y = fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))
  car_x.h = 10;
  car_x.w = 10;
  SDL_RenderFillRect(r, &car_x);
*/



  /* Affiche le tout  */
  SDL_RenderPresent(r);
}

void paintEnemies(SDL_Renderer *r,map_t *m, int t) {
  int i;
  for(i = 0; i < m->level; i++) {
    car_t voiture_e = m->cars[i][t];
    SDL_Rect rect;
    rect.x = voiture_e.x;
    rect.y = voiture_e.y;
    rect.h = voiture_e.hauteur;
    rect.w = voiture_e.largeur;
    SDL_RenderCopyEx(r, tile[voiture_e.type], NULL, &rect, voiture_e.angle, NULL, SDL_FLIP_NONE);
  }
}

