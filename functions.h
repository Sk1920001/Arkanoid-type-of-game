#include <SDL2/SDL.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Block {
  SDL_Rect rect;
  int render_state;
  struct Block* next;
}Block; 

typedef struct Ball{
  SDL_Rect rect;
  float vel_x;
  float vel_y;
}Ball;

void rotate_vector(float* x,float* y,float deg);

void check_border_collition(Ball* ball, int* pos_x_delta, int* pos_y_delta);

void check_bar_collition(Ball* ball, SDL_Rect bar, int* pos_x_delta, int* pos_y_delta);

void check_block_collition(Block* block, Ball* ball, int* pos_x_delta, int* pos_y_delta);

void deallocate(Block** head);

enum directions {LEFT, RIGHT, NONE};

