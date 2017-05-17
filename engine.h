typedef enum {
  AMBULANCE=0,
  MINITRUCK,
  TAXI,
  SPORT,
  MINIVAN,
  TRUCK,
  CAR,
  POLICE,
  VIPER,
  ALL
} type_t;

typedef struct {
  float x, y;

  int vitesse;
  int angle;

  float hauteur,largeur;

  type_t type;

} car_t;

typedef struct {
  int hauteur,largeur; //dimension de l'écran
  
  car_t voiture;

  SDL_Surface *background;
} map_t;
#define SIZE 64

int getEvent(map_t *m);
void update(map_t *m);
