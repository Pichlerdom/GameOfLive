#include "display.h"
#include "voxel.h"

Display* init_display(){

  Display *display = (Display *) calloc(1, sizeof(Display));

  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("Could not init SDL");
    return NULL;
  }

  display->window = SDL_CreateWindow(WINDOW_NAME,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
				     WINDOW_WIDTH,
				     WINDOW_HEIGHT,
				     SDL_WINDOW_SHOWN);
  if(display->window != NULL){
    display->renderer = SDL_CreateRenderer( display->window, -1,
					    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if(display->renderer == NULL){
      printf("Could not create renderer!");
      return NULL;
    }

    display->texture = SDL_CreateTexture(display->renderer,
					 SDL_PIXELFORMAT_RGBA8888,
					 SDL_TEXTUREACCESS_STREAMING,
					 WINDOW_WIDTH, WINDOW_HEIGHT);
    if(display->texture == NULL){
      printf("Could not create texture!");
      return NULL;
    }
    
  }else{
    printf("Could not create window!");
    return NULL;
  }

  display->pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  
  SDL_SetRenderDrawColor(display->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
  return display;
}

void clean_display(Display *display){
  SDL_DestroyTexture(display->texture);
  SDL_DestroyRenderer(display->renderer);
  SDL_DestroyWindow(display->window);
  SDL_Quit();
}

typedef struct{
  Voxel* front;
  Voxel* back;

}VoxelBuffers;

VoxelBuffers v_buf;

bool get_voxel_at(int x, int y){
  int x_pos, y_pos;
  
  if(x < 0){
    x_pos = (WINDOW_WIDTH) - ((int)abs(x) % (WINDOW_WIDTH));
  }else{
    x_pos = x%(WINDOW_WIDTH);
  }

  if(y < 0){
    y_pos = (WINDOW_HEIGHT) - ((int)abs(y) % (WINDOW_HEIGHT));
  }else{
    y_pos = y%(WINDOW_HEIGHT);
  }
  
  return v_buf.front[(x_pos) * WINDOW_HEIGHT + y_pos].is_active();
}

void set_voxel_at(int x, int y, bool val){
  int x_pos, y_pos;
  
  if(x < 0){
    x_pos = (WINDOW_WIDTH) - ((int)abs(x) % (WINDOW_WIDTH));
  }else{
    x_pos = x%(WINDOW_WIDTH);
  }

  if(y < 0){
    y_pos = (WINDOW_HEIGHT) - ((int)abs(y) % (WINDOW_HEIGHT));
  }else{
    y_pos = y%(WINDOW_HEIGHT);
  }
  
  v_buf.back[(x_pos) * WINDOW_HEIGHT + y_pos].set_active(val);
}

bool apply_rule(uint32_t rule, int x, int y){
  uint8_t hood = 0;
  hood |= (get_voxel_at(x,y)<<0);
  hood |= (get_voxel_at(x,y+1)<<1);
  hood |= (get_voxel_at(x,y-1)<<2);
  hood |= (get_voxel_at(x+1,y)<<3);
  hood |= (get_voxel_at(x-1,y)<<4);

  return (rule & (1<<hood))!=0;
}

void reset(){
  for(int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
    v_buf.front[i].set_active(false);
  v_buf.front[(WINDOW_WIDTH/2) * WINDOW_HEIGHT + WINDOW_WIDTH/2].set_active(true);
}

void flip(){
  Voxel *temp= v_buf.front;
  v_buf.front = v_buf.back;
  v_buf.back = temp;
}

void display_loop(uint32_t rule){
  bool key_down[8];
  for(int i = 0 ;i < 8; i ++){
    key_down[i] = false;
  }
  uint32_t currTime = SDL_GetTicks();
  uint32_t frameTime = 0;

  v_buf.front = (Voxel *) calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(Voxel));
  v_buf.back = (Voxel *) calloc(WINDOW_WIDTH * WINDOW_HEIGHT, sizeof(Voxel));
  reset();
  Display *display = init_display();

  int frame_count = 0;

  uint8_t *pixels_screen;

  SDL_Event e;
  bool run = true;
  srand(1234);
  
  while(run){
    currTime = SDL_GetTicks();

    while(SDL_PollEvent(&e)){
      
      switch(e.type)
      {
      	case SDL_KEYDOWN:
	  switch (e.key.keysym.sym){
	  case SDLK_a: key_down[A_KEY] = true;
	    break;
	  case SDLK_d: key_down[D_KEY] = true;
	    break;
	  case SDLK_w: key_down[W_KEY] = true;
	    break;
	  case SDLK_s: key_down[S_KEY] = true;
	    break;
	  case SDLK_r: key_down[R_KEY] = true;
	    reset();
	    frame_count = 0;
	    break;
	  case SDLK_f: key_down[F_KEY] = true;
	    break;
	  case SDLK_v: key_down[V_KEY] = true;
	    break;
	  case SDLK_q: key_down[Q_KEY] = true;
	    run = false;
	    break;
	  }
	  break;
	  
      case SDL_KEYUP:
	switch (e.key.keysym.sym)
	  {
	  case SDLK_a: key_down[A_KEY] = false;
	    break;
	  case SDLK_d: key_down[D_KEY] = false;
	    break;
	  case SDLK_w: key_down[W_KEY] = false;
	    break;
	  case SDLK_s: key_down[S_KEY] = false;
	    break;
	  case SDLK_r: key_down[R_KEY] = false;
	    break;
	  case SDLK_f: key_down[F_KEY] = false;
	    break;
	  case SDLK_v: key_down[V_KEY] = false;
	    break;
	  case SDLK_q: key_down[Q_KEY] = false;
	    break;
	  }
	break;
      case SDL_QUIT:
	run = false;
      }
    }

    if(frame_count%3 == 0){
      uint32_t rule_old = rule;
      if(key_down[W_KEY]) rule+=2;
      if(key_down[S_KEY]) rule-=2;
      if(key_down[F_KEY]) rule++;
      if(key_down[V_KEY]) rule--;
      if(key_down[D_KEY]) rule+=20;
      if(key_down[A_KEY]) rule-=20;
      if(rule_old != rule) reset();
    }
    printf("rule: %d", rule);
    uint32_t format;
    int32_t w,h;
    int32_t pitch;
    
    SDL_QueryTexture(display->texture, &format,NULL, &w , &h);
    
    SDL_LockTexture(display->texture, NULL, (void **) &pixels_screen, &pitch);

    uint8_t * pixel;
    for(int x = 0; x < w; x++){
      for(int y = 0; y < h; y++){
	pixel = ((uint8_t *)pixels_screen) + (y * pitch + x * 4);
	if(get_voxel_at(x,
			y)){
	  pixel[0] = 255;
	  pixel[1] = 255;
	  pixel[2] = 255;
	  pixel[3] = 255;
	}else{
	  pixel[0] = 0;
	  pixel[1] = 0;
	  pixel[2] = 0;
	  pixel[3] = 0;
	}
	set_voxel_at(x, y, apply_rule(rule, x, y));
      }
    }
    flip();

    SDL_UnlockTexture(display->texture);
    
    SDL_Rect rect = (SDL_Rect){0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
    
    //Update screen
    SDL_RenderCopy(display->renderer, display->texture,NULL, &rect);
    SDL_RenderPresent( display->renderer );
   

    //FPS stuff
    frameTime = SDL_GetTicks() - currTime;
    printf("\nmspf = %d\n",frameTime);
    if(frameTime > MS_PER_FRAME){
      frameTime = MS_PER_FRAME;
    }

    SDL_Delay(MS_PER_FRAME-frameTime);
    frame_count++;
  }
  
  clean_display(display);
}
