#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "engine.h"
#include "graphics.h"
#include "timer.h"

/* Toutes les tuiles du jeu */
SDL_Texture *tile[ALL];
SDL_Texture *tile_background;
SDL_Rect rect_flammes[NB_FLAMMES];
SDL_Texture *tile_flammes;
SDL_Texture *tile_level[LEVEL];
SDL_Texture *tile_police[3];
SDL_Texture *tile_bonus[4];

SDL_Rect rect_tigres[16];
SDL_Texture *tile_tigres;
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

const char *tilelevelnames[]={
  "data/1.bmp",
  "data/2.bmp",
  "data/3.bmp",
  "data/4.bmp",
  "data/5.bmp",
  "data/6.bmp",
  "data/7.bmp",
  "data/8.bmp",
  "data/9.bmp",
  "data/10.bmp",
  "data/11.bmp",
  "data/12.bmp",
  "data/13.bmp",
  "data/14.bmp",
  "data/15.bmp",
  "data/16.bmp",
  "data/17.bmp",
  "data/18.bmp",
  "data/19.bmp",
  "data/20.bmp"
};

const char *tilepolicenames[]={
  "data/police1.bmp",
  "data/police2.bmp",
  "data/police3.bmp"
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
  int i;
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

void loadTilesLevel(SDL_Renderer *s) {
  int i;
  SDL_SetRenderDrawColor(s, 0, 0, 0, 0);
  for (i=0; i<LEVEL; i++)  {
    SDL_Surface *loadedImage=SDL_LoadBMP(tilelevelnames[i]);
    if (loadedImage !=NULL) {
      Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
      SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
      tile_level[i]=SDL_CreateTextureFromSurface(s, loadedImage );
      SDL_FreeSurface(loadedImage);
    } else fprintf(stderr,"Missing file %s:%s\n",tilelevelnames[i],SDL_GetError());
  }

  for (i=0; i<3; i++)  {
    SDL_Surface *loadedImage=SDL_LoadBMP(tilepolicenames[i]);
    if (loadedImage !=NULL) {
      Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
      SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
      tile_police[i]=SDL_CreateTextureFromSurface(s, loadedImage );
      SDL_FreeSurface(loadedImage);
    } else fprintf(stderr,"Missing file %s:%s\n",tilepolicenames[i],SDL_GetError());
  }
}

void loadTilesBonus(SDL_Renderer *s) {

  // BONUS VITESSE
  SDL_Surface *loadedImage=SDL_LoadBMP("data/lievre.bmp");
  if (loadedImage !=NULL) {
    Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
    SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
    tile_bonus[0]=SDL_CreateTextureFromSurface(s, loadedImage );
    SDL_FreeSurface(loadedImage);
  } else fprintf(stderr,"Missing file %s:%s\n","data/lievre.bmp",SDL_GetError());

  // BONUS ETOILE
  loadedImage=SDL_LoadBMP("data/etoile.bmp");
  if (loadedImage !=NULL) {
    Uint32 colorkey = SDL_MapRGB(loadedImage->format,0x00,0x00,0x00);
    SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
    tile_bonus[1]=SDL_CreateTextureFromSurface(s, loadedImage );
    SDL_FreeSurface(loadedImage);
  } else fprintf(stderr,"Missing file %s:%s\n","data/etoile.bmp",SDL_GetError());

  // BONUS EPIC SONG
  loadedImage=SDL_LoadBMP("data/note_musique.bmp");
  if (loadedImage !=NULL) {
    Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
    SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
    tile_bonus[2]=SDL_CreateTextureFromSurface(s, loadedImage );
    SDL_FreeSurface(loadedImage);
  } else fprintf(stderr,"Missing file %s:%s\n","data/note_musique.bmp",SDL_GetError());

  // BONUS LALOUX
  loadedImage=SDL_LoadBMP("data/laloux.bmp");
  if (loadedImage !=NULL) {
    Uint32 colorkey = SDL_MapRGB(loadedImage->format,0xff,0xff,0xff);
    SDL_SetColorKey(loadedImage,SDL_TRUE,colorkey);
    tile_bonus[3]=SDL_CreateTextureFromSurface(s, loadedImage );
    SDL_FreeSurface(loadedImage);
  } else fprintf(stderr,"Missing file %s:%s\n","data/laloux.bmp",SDL_GetError());
}
/* Charge toutes les tuiles du jeu
   DEJA ECRIT
 */
void loadFlammes(SDL_Renderer *s) {
  int i,j, indice;
  SDL_SetRenderDrawColor(s, 0, 0, 0, 0);
  SDL_Surface *loadedImage1=SDL_LoadBMP("data/flammes.bmp");
  Uint32 colorkey1 = SDL_MapRGB(loadedImage1->format,0x00,0x00,0x00);
  SDL_SetColorKey(loadedImage1,SDL_TRUE,colorkey1);
  tile_flammes=SDL_CreateTextureFromSurface(s, loadedImage1 );

  if (loadedImage1 !=NULL) {
    for (i=0; i<5; i++)  {
      for (j=0; j<5; j++)  {

        indice = j+i*5;
        rect_flammes[indice].x = j*FLAMME_SIZE;
        rect_flammes[indice].y = i*FLAMME_SIZE;
        rect_flammes[indice].w = FLAMME_SIZE;
        rect_flammes[indice].h = FLAMME_SIZE;
      } 
    } 
    SDL_FreeSurface(loadedImage1);
  } else fprintf(stderr,"Missing file %s:%s\n","data/flammes.bmp",SDL_GetError());


  SDL_SetRenderDrawColor(s, 0, 0, 0, 0);
  SDL_Surface *loadedImage2=SDL_LoadBMP("data/tigres.bmp");
  Uint32 colorkey2 = SDL_MapRGB(loadedImage2->format,0xff,0xff,0xff);
  SDL_SetColorKey(loadedImage2,SDL_TRUE,colorkey2);
  tile_tigres=SDL_CreateTextureFromSurface(s, loadedImage2 );
  if (loadedImage2 !=NULL) {
    for (i=0; i<4; i++)  {
      for (j=0; j<4; j++)  {

        indice = j+i*4;
        rect_tigres[indice].x = j*56;
        rect_tigres[indice].y = i*56;
        rect_tigres[indice].w = 56;
        rect_tigres[indice].h = 56;
      } 
    } 
    SDL_FreeSurface(loadedImage2);
  } else fprintf(stderr,"Missing file %s:%s\n","data/tigres.bmp",SDL_GetError());
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

  m->voiture.hauteur = SIZE;
  m->voiture.largeur = SIZE;

  m->voiture.angle = 0;

  m->dist_klakson = 250.0;

  m->tigre_p.x = 450;
  m->tigre_p.y = 420;
  m->dir_tigre = 1;

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
void paint(SDL_Renderer *r,const map_t *m, int t) {
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

  paintTigres(r, m, t);
  
  switch(m->getBonus) {
    case -1:
      paintBonus(r, m); break;
    case 3:
      showLaloux(r, m); break;
    default: break;
  }


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
  //SDL_RenderFillRect(r, &rect_src);

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
  
  if(m->getBonus == 1) {
    SDL_SetRenderDrawColor(r, 255, 255, 0, 255 );
    SDL_RenderDrawRect(r, &rect);
  }

  paintFlammes(r,m->voiture);
  paintEnemies(r,m,t);
  paintPolice(r,m,t);

  double angle = m->voiture.angle-90;
  double rad = angle * M_PI / 180.0;

  SDL_SetRenderDrawColor(r, 255, 0, 255, 255 );
  SDL_Rect car_x1;
  car_x1.x = m->voiture.x-5; //fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(-rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
  car_x1.y = m->voiture.y-5; //fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))
  car_x1.h = 10;
  car_x1.w = 10;
  //SDL_RenderFillRect(r, &car_x1);

  SDL_SetRenderDrawColor(r, 0, 255, 255, 255 );
  SDL_Rect car_x2;
  car_x2.x = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad + M_PI/4) -2; //fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(-rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
  car_x2.y = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad + M_PI/4) -2; //fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))
  car_x2.h = 4;
  car_x2.w = 4;
  SDL_RenderFillRect(r, &car_x2);

  SDL_SetRenderDrawColor(r, 255, 0, 255, 255 );
  SDL_Rect car_x3;
  car_x3.x = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad - M_PI/4) -2; //fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(-rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
  car_x3.y = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad - M_PI/4) -2; //fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))
  car_x3.h = 4;
  car_x3.w = 4;
  SDL_RenderFillRect(r, &car_x3);

  if(t == 0) {
    SDL_Rect passp;
    passp.x = m->largeur/2 - 200;
    passp.y = m->hauteur/2 - 200;
    passp.h = 400;
    passp.w = 400;
    SDL_RenderCopyEx(r, tile_level[m->level], NULL, &passp, 0, NULL, SDL_FLIP_NONE);
  }


  /* Affiche le tout  */
  SDL_RenderPresent(r);
}

void paintEnemies(SDL_Renderer *r,const map_t *m, int t) {
  int i;
  for(i = 0; i < m->level; i++) {
    car_t voiture_e = m->cars[i][t];

    SDL_Rect rect;
    rect.x = voiture_e.x;
    rect.y = voiture_e.y;
    rect.h = voiture_e.hauteur;
    rect.w = voiture_e.largeur;
    SDL_RenderCopyEx(r, tile[voiture_e.type], NULL, &rect, voiture_e.angle, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255 );
    SDL_RenderDrawRect(r, &rect);

    paintFlammes(r,voiture_e);
  }
}

void paintPolice(SDL_Renderer *r,const map_t *m, int t) {
  int i;
  for(i = 0; i < m->level; i++) {
    car_t voiture_e = m->police[i][t];
    SDL_Rect rect;
    rect.x = voiture_e.x;
    rect.y = voiture_e.y;
    rect.h = voiture_e.hauteur;
    rect.w = voiture_e.largeur;
    SDL_RenderCopyEx(r, tile_police[t%3], NULL, &rect, voiture_e.angle, NULL, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255 );
    SDL_RenderDrawRect(r, &rect);
  }
}

void paintFlammes(SDL_Renderer *r,const car_t c) {
  if(c.collision) {
    SDL_Rect rect;
    rect.x = c.x - c.hauteur;
    rect.y = c.y - c.hauteur;
    rect.w = FLAMME_SIZE*1.5;
    rect.h = FLAMME_SIZE*1.5;

    SDL_RenderCopyEx(r,tile_flammes,&rect_flammes[NB_FLAMMES-1-c.collision],&rect, 0,NULL, SDL_FLIP_NONE);  
  }
}

void paintTigres(SDL_Renderer *r, map_t *m, int t) {
  if(m->tigre_p.x >= 750) {
    m->dir_tigre = 0;
  } else if (m->tigre_p.x <= 425) {
    m->dir_tigre = 1;
  }


  if(m->dir_tigre == 1) {
    m->tigre_p.x++;
  } else {
    m->tigre_p.x--;
  }

  SDL_Rect rect;
  rect.x = m->tigre_p.x;
  rect.y = m->tigre_p.y;
  rect.w = 56;
  rect.h = 56;
  //SDL_RenderCopyEx(r,tile_tigres,&rect_tigres[6],&rect, 0,NULL, SDL_FLIP_NONE);  

  SDL_RenderCopyEx(r,tile_tigres,&rect_tigres[4 + m->dir_tigre*4 + ((t/4)%4)],&rect, 0,NULL, SDL_FLIP_NONE);  
}


void paintBonus(SDL_Renderer *r, map_t *m) {
  SDL_Rect rect;
  rect.x = m->bonus[m->rang_bonus][0] - 20;
  rect.y = m->bonus[m->rang_bonus][1] - 20;
  rect.w = 40;
  rect.h = 40;
  SDL_SetRenderDrawColor(r, 125, 0, 255, 255 );
  SDL_RenderFillRect(r, &rect);
  SDL_RenderCopyEx(r,tile_bonus[m->typeBonus],NULL,&rect, 0,NULL, SDL_FLIP_NONE);  
}

void showLaloux(SDL_Renderer *r, map_t *m) {
  int direction_horizontale = rand() % 2;
  int direction_vecticale = rand() % 2;

  int rand_h = rand() % 4;
  int rand_v = rand() % 4;

  m->photo_laloux.x = (direction_horizontale == 0) ? rand_h : m->photo_laloux.x-rand_h;
  m->photo_laloux.y = (direction_vecticale == 0) ? rand_v : m->photo_laloux.y-rand_v;

  m->lalouxSize += 2;

  SDL_Rect rect;
  rect.x = m->photo_laloux.x - 40;
  rect.y = m->photo_laloux.y - 40;
  rect.w = m->lalouxSize;
  rect.h = m->lalouxSize;
  SDL_RenderCopyEx(r,tile_bonus[m->typeBonus],NULL,&rect, 0,NULL, SDL_FLIP_NONE);  

}