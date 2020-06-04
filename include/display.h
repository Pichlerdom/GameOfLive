#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include<SDL2/SDL.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdlib.h>
#include<string>
#include<time.h>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 1080 

#define MS_PER_FRAME 15

#define WINDOW_NAME "Planets"

enum KEYS{W_KEY, A_KEY, S_KEY, D_KEY,R_KEY,F_KEY,V_KEY,Q_KEY};

typedef struct{
  SDL_Renderer* renderer;
  SDL_Window* window;
  SDL_Texture* texture;
  SDL_PixelFormat *pixelFormat;
  int x;
  int y;
}Display;

void display_loop(uint32_t rule);




#endif
