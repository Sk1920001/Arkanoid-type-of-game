#include <SDL2/SDL.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_H 480
#define WINDOW_W 640

typedef enum {RED, GREEN, YELLOW, PURPLE} Colors;

typedef struct Block {
  SDL_Rect rect;
  struct Block* next;
  Colors color; 
}Block; 

typedef struct Ball{
  SDL_Rect rect;
  float vel_x;
  float vel_y;
}Ball;

void rotate_vector(float* x,float* y,float deg);

void check_border_collition(Ball* ball, int* pos_x_delta, int* pos_y_delta);

void check_bar_collition(Ball* ball, SDL_Rect bar, int* pos_x_delta, int* pos_y_delta);

void check_block_collition(Block** head, Block* block, Ball* ball, int* pos_x_delta, int* pos_y_delta);

void deallocate(Block** head);

void delete_block(Block** head, int pos_x, int pos_y);

void create_lvl1(Block** head);

