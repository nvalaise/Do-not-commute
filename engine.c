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
      case SDLK_SPACE:
        if(m->temps - m->t_klakson > 200)
          klakson(m);
      case SDLK_p:
        m->pause = (m->pause == 0) ? 1 : 0; 
        break;
      default:
        break;
      }
    }
  }
  return 0;
}

void loadCheckpoints(map_t *m) {
  int i,j;
  int compteur = 0;
  // compte le nombre de checkpoints
  for (i=0; i < m->largeur; i++) {
    for (j=0; j < m->hauteur; j++) {
      compteur = getpixel(m->background,i,j) == 0x00f ? compteur + 1 : compteur;
    }
  }

  m->nb_checkpoints = compteur;
  m->checkpoints = (int**)malloc(sizeof(int*)*m->nb_checkpoints);

  // pour chaque chepoints, allocation de 2 cases
  for(i=0; i < compteur; i++) {
    m->checkpoints[i] = (int*) malloc(sizeof(int)*2);
  }


  // attribution des coordonnées
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

  // compte le nombre de bonus
  compteur = 0;
  for (i=0; i < m->largeur; i++) {
    for (j=0; j < m->hauteur; j++) {
      compteur = getpixel(m->background,i,j) == 0xcea ? compteur + 1 : compteur;
    }
  }

  m->nb_bonus = compteur;
  m->bonus = (int**)malloc(sizeof(int*)*m->nb_bonus);

  // pour chaque chepoints, allocation de 2 cases
  for(i=0; i < compteur; i++) {
    m->bonus[i] = (int*) malloc(sizeof(int)*2);
  }


  // attribution des coordonnées
  compteur = 0;
  for (i=0; i < m->largeur; i++) {
    for (j=0; j < m->hauteur; j++) {
      if(getpixel(m->background,i,j) == 0xcea) {
        m->bonus[compteur][0] = i;
        m->bonus[compteur][1] = j;
        compteur=compteur+1;
      }
    }
  }

  printf("%d Bonus\n", m->nb_bonus);
  m->pause = 0;
}

/* A COMPLETER */
void update(map_t *m) {
  performedCar(m);

  m->voiture.temps = m->temps;
  int t = m->temps;
  m->cars[m->level][m->voiture.temps] = m->voiture;


  int center0_x = fabsf((int) m->voiture.x + m->voiture.largeur/2);
  int center0_y = fabsf((int) m->voiture.y + m->voiture.hauteur/2);
  int angle0 = (m->voiture.angle-90) * M_PI / 180.0;
/*
  int i,j;
  for(i=0; i<m->level; i++) {
    car_t voiture_e = m->cars[i][t];
    
    int center1_x = voiture_e.x + voiture_e.largeur/2;
    int center1_y = voiture_e.y + voiture_e.hauteur/2;

    int angle1 = (voiture_e.angle-90) * M_PI / 180.0;

    if(check_collision(center0_x, center0_y, angle0, center1_x, center1_y, angle1)) {

      printf("collision %d\n", t);
      printf("%f - %f\n", m->cars[i][t].x, m->cars[i][t].y);
      m->cars[i][t].x += 4 * cos(angle0) * m->voiture.vitesse; 
      m->cars[i][t].y += 4 * sin(angle0) * m->voiture.vitesse; 
      //m->cars[i][t].angle = voiture_e.angle;
      printf("%f - %f\n", m->cars[i][t].x, m->cars[i][t].y);
    }
  }
*/
}

void initGame(map_t *m) {


  // Chekpoints
  srand(time(NULL));
  m->rang_checkpoints_src = rand() % m->nb_checkpoints;

  do {
    m->rang_checkpoints_dest = rand() % m->nb_checkpoints;
  } while(m->rang_checkpoints_src == m->rang_checkpoints_dest);


  m->voiture.x = m->checkpoints[m->rang_checkpoints_src][0]-20;
  m->voiture.y = m->checkpoints[m->rang_checkpoints_src][1]-20;

  m->voiture.checkpoints_src = m->rang_checkpoints_src;
  m->voiture.checkpoints_dest = m->rang_checkpoints_dest;

  srand(time(NULL));
  int rand_type = rand() % 9;

  // Type de la voiture
  m->voiture.type = (type_t)rand_type;
  m->voiture.vitesse = 2;

  // Type du Bonus
  srand(time(NULL));
  m->rang_bonus = rand() % m->nb_bonus;
  m->typeBonus = rand() % 4;

  m->getBonus = -1;

  m->photo_laloux.x = m->largeur/2 - 40;
  m->photo_laloux.y = m->hauteur/2 - 40;
  m->lalouxSize = 80;


  // Gestion de la police
  int deltaCollision, delta = 150;
  int i,j, ind;
  for(i=1;i<m->level; i++) {
    deltaCollision = 0;

    if(m->boolPolice[i]) {

      for(j=0;j<NB_TEMPS; j++) {
        car_t copy = m->cars[i][j];

        if(j < delta + 1) {
          m->police[i][j] = m->cars[i][0];
        } else {
          if (copy.collision == NB_FLAMMES-1 && deltaCollision+NB_FLAMMES < delta && j>delta-deltaCollision) {
            deltaCollision+=NB_FLAMMES;
          }

          ind = j - delta + deltaCollision;
          m->police[i][j] = m->cars[i][ind];
        }
      }
    }
  }
}

void performedCar(map_t *m) {
  int width, height, i;
  width = m->largeur-m->voiture.largeur;
  height = m->hauteur-m->voiture.hauteur;

  float origine_x, origine_y;
  origine_x = m->voiture.x;
  origine_y = m->voiture.y;

  int center_x = fabsf((int) m->voiture.x + m->voiture.largeur/2);
  int center_y = fabsf((int) m->voiture.y + m->voiture.hauteur/2);

  double angle = m->voiture.angle-90;
  double rad = angle * M_PI / 180.0;

  int angle0 = (m->voiture.angle-90) * M_PI / 180.0;

  int collision = 0;
  if(m->getBonus != 1) {
    for(i=0; i<m->level; i++) {
      car_t voiture_e = m->cars[i][m->temps];
      
      int center1_x = voiture_e.x + voiture_e.largeur/2;
      int center1_y = voiture_e.y + voiture_e.hauteur/2;

      int angle1 = (voiture_e.angle-90) * M_PI / 180.0;
      int delta = 150;
      if(check_collision(center_x, center_y, rad, center1_x, center1_y, angle1) && !m->voiture.collision && m->getBonus != 1) {
        collision = 1;
        m->voiture.collision = NB_FLAMMES;
        m->boolPolice[m->level] = 1;
      }
    }
  }


/*

  int test1_x = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad + M_PI/4); // + cos(rad) + (1-sin(rad))
  int test1_y = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad + M_PI/4); //+ sin(rad) + (1-cos(rad))

  int test2_x = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad - M_PI/4); // + cos(rad) + (1-sin(rad))
  int test2_y = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad - M_PI/4); //+ sin(rad) + (1-cos(rad))


*/
  int test_x = abs((int) fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(rad) * m->voiture.largeur)/2 ); // + cos(rad) + (1-sin(rad))
  int test_y = abs((int) fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2); //+ sin(rad) + (1-cos(rad))

int x1 = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad + M_PI/4); //fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(-rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
int y1 = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad + M_PI/4); //fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))

int x2 = (m->voiture.x + (m->voiture.largeur)/2) + (m->voiture.largeur)/2 * cos(rad - M_PI/4); //fabsf((int) m->voiture.x + m->voiture.largeur/2) + (cos(-rad) * m->voiture.largeur)/2 - 5; // + cos(rad) + (1-sin(rad))
int y2 = (m->voiture.y + (m->voiture.hauteur)/2) + (m->voiture.hauteur)/2 * sin(rad - M_PI/4); //fabsf((int) m->voiture.y + m->voiture.hauteur/2) + (sin(rad) * m->voiture.hauteur)/2 - 5; //+ sin(rad) + (1-cos(rad))


  int pixel1 = getpixel(m->background,center_x,center_y);

  int pixel2 = getpixel(m->background,x1,y1);
  int pixel3 = getpixel(m->background,x2,y2);

  if(m->voiture.collision) {
    //m->voiture.x -= 3*cos(rad) * m->voiture.vitesse;
    //m->voiture.y -= 3*sin(rad) * m->voiture.vitesse;
    m->voiture.x = origine_x;
    m->voiture.y = origine_y;
  }
  else if ((pixel2 > 0x000 && pixel2 < 0x999) || (pixel3 > 0x000 && pixel3 < 0x999)) { // mur
    m->voiture.x = origine_x;
    m->voiture.y = origine_y;
  } else if (getpixel(m->background,center_x,center_y) == 0xcea ) { // herbe
    m->voiture.x += cos(rad) * m->voiture.vitesse / 2;
    m->voiture.y += sin(rad) * m->voiture.vitesse / 2;

  } else { // route
    m->voiture.x += cos(rad) * m->voiture.vitesse;
    m->voiture.y += sin(rad) * m->voiture.vitesse;
  }

  if(m->voiture.collision) {
    m->voiture.collision--;
  }


  if (m->voiture.x <= 0){
    m->voiture.x = 0;
  }
  if (m->voiture.x >= width){
    m->voiture.x = width;
  }
  if (m->voiture.y <= 10){
    m->voiture.y = 10;
  }
  if (m->voiture.y >= height){
    m->voiture.y = height;
  }
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

  if(PointInRect(&car_center, &rect_dest)) {
    return 1;
  }
  return 0;
}

int checkBonus(map_t *m) {
  SDL_Rect rect;
  rect.x = m->bonus[m->rang_bonus][0] - 20;
  rect.y = m->bonus[m->rang_bonus][1] - 20;
  rect.w = 40;
  rect.h = 40;

  int center_x = fabsf((int) m->voiture.x + m->voiture.largeur/2);
  int center_y = fabsf((int) m->voiture.y + m->voiture.hauteur/2);

  SDL_Point car_center;
  car_center.x = center_x;
  car_center.y = center_y;

  if(PointInRect(&car_center, &rect)) {
    printf("Bonus !!!!\n");
    m->getBonus = m->typeBonus;
  }
  return m->getBonus;
}

int PointInRect(const SDL_Point *p, const SDL_Rect *r)
{
    return ( (p->x >= r->x) && (p->x < (r->x + r->w)) &&
             (p->y >= r->y) && (p->y < (r->y + r->h)) ) ? 1 : 0;
}



void klakson(map_t *m) {
  int i, j;
  const int delta = 200;
  const int t = m->temps;

  const float car_center_x = m->voiture.x + m->voiture.largeur/2;
  const float car_center_y = m->voiture.y + m->voiture.hauteur/2;

  float center_x, center_y;
  for(i = 0; i < m->level; i++) {
    car_t voiture_e = m->cars[i][t];

    center_x = voiture_e.x + voiture_e.largeur/2;
    center_y = voiture_e.y + voiture_e.hauteur/2;
    
    if(sqrt(pow((car_center_x-center_x),2) + pow((car_center_y-center_y),2.0) ) < m->dist_klakson) {
      m->t_klakson = m->temps; 

      for(j=t;j+delta<NB_TEMPS;j++) { // décalage

        if(m->cars[i][t].temps == 0) {
          break;
        }
        m->cars[i][j+delta] = m->cars[i][j];
        m->cars[i][j+delta].temps += delta;
      }

      for(j=t;j<t+delta && j<NB_TEMPS;j++) { // arret
          m->cars[i][j] = m->cars[i][t];
          m->cars[i][j].temps += delta;
      }

      printf("DETECTED : %d : %d (%f px)\n", t, i, sqrt(pow((car_center_x-center_x),2) + pow((car_center_y-center_y),2.0) ));
    }
  }
}

/* Teste si 2 voitures sont en collision. 
   x0,y0: position du centre de la premiere voiture
   a0: angle en radian de la premiere voiture 
   x1,y1: position du centre de la deuxieme voiture
   a1: angle en radian de la deuxieme voiture 
   On suppose que la voiture est verticale lorsque l'angle vaut 0.

   Retourne: 0 si pas de collision, 1 si collision
*/
int check_collision(float x0,float y0,float a0,float x1,float y1,float a1) {
  int i;
  float SIZEX=SIZE*0.34;
  float SIZEY=SIZE*0.75;
  float ca=cos(a0 - a1);
  float sa=sin(a0-a1);
  float cx=x0-x1;
  float cy=y0-y1;
  float xx=cx*cos(-a1)-cy*sin(-a1);
  float yy=cx*sin(-a1)+cy*cos(-a1);                            
  float tx[]= {-SIZEX/2, SIZEX/2,-SIZEX/2,SIZEX/2};
  float ty[]={-SIZEY/2, SIZEY/2,SIZEY/2,-SIZEY/2};
  for (i=0; i<4; i++) {
    float dx=xx+ca*tx[i]-sa*ty[i];
    float dy=yy+sa*tx[i]+ca*ty[i];
    if (dx>=-SIZEX/2&&dx<=SIZEX/2&&dy>=-SIZEY/2&&dy<=SIZEY/2) return 1;
  }
  return 0;
}
