void loadTiles(SDL_Renderer *s, const map_t *m);
void loadFlammes(SDL_Renderer *s);
void loadTilesLevel(SDL_Renderer *s);
void loadTilesBonus(SDL_Renderer *s);
map_t *loadMap(char *filename);
SDL_Renderer *openWindow(int w,int h);
void paint(SDL_Renderer *s, map_t *m);
void paintEnemies(SDL_Renderer *s,const map_t *m, int t);
void paintPolice(SDL_Renderer *s,const map_t *m, int t);
void paintFlammes(SDL_Renderer *r,const car_t car);
void paintKlakson(SDL_Renderer *s,const map_t *m);
void paintTigres(SDL_Renderer *r, map_t *m, int t);
void paintBonus(SDL_Renderer *r, map_t *m);
void showLaloux(SDL_Renderer *r, map_t *m);
void paintMenu(SDL_Renderer *r, map_t *m);

void paintEnd(SDL_Renderer *r, map_t *m, int win);