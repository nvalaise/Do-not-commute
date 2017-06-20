#define M_PI 3.14159265358979323846
#define TEMPS_MAX 30
#define NB_FLAMMES 25
#define FLAMME_SIZE 64
#define NB_TEMPS 20000
#define LEVEL 13
#define DELTA_ENNEMIES 10

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
} bonus_t;

typedef enum {
  VITESSE = 0,
  ETOILE = 1,
  EPIC_SONG = 2,
  TETE_DE_LALOUX = 3,
  STOP_POLICE = 4// à voire
} type_t;

typedef struct {
  float x, y;

  int checkpoints_src, checkpoints_dest;

  int vitesse;
  int angle;

  float hauteur,largeur;

  type_t type;

  int temps;

  int collision;

} car_t;

typedef struct {
  int type_menu;
  int pause;

  int hauteur,largeur; //dimension de l'écran
  
  car_t voiture;

  car_t cars[LEVEL][NB_TEMPS];

  int level;

  SDL_Surface *background;

  int** checkpoints;
  int nb_checkpoints;
  int rang_checkpoints_src;
  int rang_checkpoints_dest;


  int** bonus;
  int nb_bonus;
  int rang_bonus;
  int typeBonus;

  int getBonus;
  SDL_Point photo_laloux;
  int lalouxSize;


  int boolPolice[LEVEL];
  car_t police[LEVEL][NB_TEMPS];

  int temps;

  int t_klakson;
  float dist_klakson;
  int boolKlakson;

  SDL_Point tigre_p;
  int dir_tigre;


} map_t;

#define SIZE 32

int getEvent(map_t *m);
void loadCheckpoints(map_t *m);
void update(map_t *m);
void performedCar(map_t *m);
int carArriveInDestination(map_t *m);
void initGame(map_t *m);
int checkBonus(map_t *m);

car_t setCarType(type_t type);
void reinitCars(map_t *m);
int checkPoliceCatchCar(map_t *m, int t);

int PointInRect(const SDL_Point *p, const SDL_Rect *r);
void klakson(map_t *m);
int check_collision(float x0,float y0,float a0,float x1,float y1,float a1);