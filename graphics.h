void loadTiles(SDL_Renderer *s, const map_t *m);
map_t *loadMap(char *filename);
SDL_Renderer *openWindow(int w,int h);
void paint(SDL_Renderer *s,map_t *m, int t);
inline void paintEnemies(SDL_Renderer *s,map_t *m, int t);