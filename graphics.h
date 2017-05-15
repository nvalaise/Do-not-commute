void loadTiles(SDL_Renderer *s);
map_t *loadMap(char *filename);
SDL_Renderer *openWindow(int w,int h);
void paint(SDL_Renderer *s,map_t *m);
