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
  /* A COMPLETER */
} car_t;

typedef struct {
  /* A COMPLETER */
} map_t;
#define SIZE 64

int getEvent(map_t *m);
void update(map_t *m);
