#define M_PI 3.14159265358979323846
#define TEMPS_MAX 90

typedef enum {
  AMBULANCE=0,
  MINITRUCK=1,
  TAXI=2,
  SPORT=3,
  MINIVAN=4,
  TRUCK=5,
  CAR=6,
  POLICE=7,
  VIPER=8,
  ALL=9
} type_t;

typedef struct {
  float x, y;

  int checkpoints_src, checkpoints_dest;

  int vitesse;
  int angle;

  float hauteur,largeur;

  type_t type;

  int temps;

} car_t;

typedef struct {
  int hauteur,largeur; //dimension de l'écran
  
  car_t voiture;

  car_t cars[20][5000];

  int level;

  SDL_Surface *background;

  int** checkpoints;
  int nb_checkpoints;
  int rang_checkpoints_src;
  int rang_checkpoints_dest;

  int temps;
  int temps_1; // horloge considérée comme constante, temps du décalage avec le temps d'initialisation de la position (où le joueur peut s'orienter)

} map_t;

//#define SIZE 64

int getEvent(map_t *m);
void loadCheckpoints(map_t *m);
void update(map_t *m);
void performedCar(map_t *m);
int carArriveInDestination(map_t *m);
void initGame(map_t *m);

int PointInRect(const SDL_Point *p, const SDL_Rect *r);
