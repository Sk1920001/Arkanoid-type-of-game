#include "functions.h"

int main(void){
  
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  SDL_Rect bar = {320,460,60,10};
  Ball ball;
  ball.rect.x = 340;
  ball.rect.y = 455;
  ball.rect.w = 5;
  ball.rect.h = 5;
  ball.vel_x = -(1/sqrt(2))*8;
  ball.vel_y = -(1/sqrt(2))*8;

  Block* head = malloc(sizeof(Block));
  Block* second_block = malloc(sizeof(Block));
  Block* third_block = malloc(sizeof(Block));

  head->render_state = 1;
  head->rect.x = 100;
  head->rect.y = 100;
  head->rect.w = 40;
  head->rect.h = 10;
  head->next = second_block;

  second_block->render_state = 1;
  second_block->rect.x = 200;
  second_block->rect.y = 100;
  second_block->rect.w = 40;
  second_block->rect.h = 10;
  second_block->next = third_block;

  third_block->render_state = 1;
  third_block->rect.x = 300;
  third_block->rect.y = 100;
  third_block->rect.w = 40;
  third_block->rect.h = 10;
  third_block->next = NULL;


  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WINDOW_W,WINDOW_H, 0, &window, &renderer);

  int running = 1;
  enum directions dir;
  int game_start = 0;

  int pos_x_delta = 0;
  int pos_y_delta = 0;

  int mouse_pos_x,mouse_pos_y;

  while(running){
    
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT) running = 0;
      else if(e.type == SDL_MOUSEBUTTONDOWN){
        if(e.button.button == SDL_BUTTON_LEFT & !game_start){
          game_start = 1;
        }
      }
      SDL_GetMouseState(&mouse_pos_x,&mouse_pos_y);
      if(mouse_pos_x + bar.w <= WINDOW_W){
        bar.x = mouse_pos_x;
      }else{
        bar.x = WINDOW_W - bar.w;
      }

    }

    pos_x_delta = 0;
    pos_y_delta = 0;

    check_border_collition(&ball, &pos_x_delta, &pos_y_delta);

    check_bar_collition(&ball, bar, &pos_x_delta, &pos_y_delta);

    for(Block* curr = head; curr != NULL; curr = curr->next){
      if(curr->render_state){
        check_block_collition(curr,&ball,&pos_x_delta,&pos_y_delta);
      }
    }

    if(ball.rect.y >= 480){
      running = 0;
    }

    if(game_start){

      // fix bug with bar collition 
      
      ball.rect.x += ball.vel_x + 2*pos_x_delta;
      ball.rect.y += ball.vel_y + 2*pos_y_delta;

    }else{
      ball.rect.x = bar.x + 2*ball.rect.w;
      ball.rect.y = bar.y - ball.rect.h;

    }

    printf("posicion x del mouse: %i\n",mouse_pos_x);
    printf("posicion y del mouse: %i\n",mouse_pos_y);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    SDL_RenderFillRect(renderer,&bar);

    SDL_SetRenderDrawColor(renderer,128,0,128,255);
    for(Block* curr = head; curr != NULL; curr = curr->next){
      if(curr->render_state){
        SDL_RenderFillRect(renderer,&(curr->rect));    
      }
    }

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&(ball.rect));

    SDL_RenderPresent(renderer);
    SDL_Delay(30);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
