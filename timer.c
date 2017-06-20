#include <SDL2/SDL.h>
#define TICK_INTERVAL 20

Uint32 next;
Uint32 timerJeux;


void timerInit() {
  next= SDL_GetTicks() + TICK_INTERVAL;
  timerJeux = SDL_GetTicks() + TICK_INTERVAL;
}

void timerWait() {
  Uint32 now;
  now = SDL_GetTicks();
  if(next>now) SDL_Delay(next-now);
  next+=TICK_INTERVAL;
}

void timerJeuxWait() {
  Uint32 now;
  now = SDL_GetTicks();
  if(timerJeux>now) SDL_Delay(timerJeux-now);
  timerJeux+=TICK_INTERVAL;
}

void reInitTimer() {
	timerJeux = 0;
}

Uint32 getNext(){
	return next;
}

Uint32 getTimerJeux(){
	return timerJeux;
}
