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
  ball.vel_x = -(1/sqrt(2))*5;
  ball.vel_y = -(1/sqrt(2))*5;

  Block* head = NULL;
  create_lvl1(&head);
  
  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WINDOW_W,WINDOW_H, 0, &window, &renderer); NULL;
  

  int running = 1;
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
      //fix bug when bar pass the possition of the ball, the ball does wierd things :c
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
      check_block_collition(&head,curr,&ball,&pos_x_delta,&pos_y_delta);
    }

    if(ball.rect.y >= 480 || head == NULL){
      running = 0;
    }

    if(game_start){

      // fix bug with bar collition 
      // is already fixed ;]
      
      ball.rect.x += ball.vel_x + 2*pos_x_delta;
      ball.rect.y += ball.vel_y + 2*pos_y_delta;

    }else{
      ball.rect.x = bar.x + 2*ball.rect.w;
      ball.rect.y = bar.y - ball.rect.h;

    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    SDL_RenderFillRect(renderer,&bar);

    for(Block* curr = head; curr != NULL; curr = curr->next){
      switch (curr->color) {
        case RED:
          SDL_SetRenderDrawColor(renderer,255,0,0,255);
          break;
        case GREEN:
          SDL_SetRenderDrawColor(renderer,0,128,0,255);
          break;
        case YELLOW:
          SDL_SetRenderDrawColor(renderer,255,255,0,255);
          break;
        case PURPLE:
          SDL_SetRenderDrawColor(renderer,128,0,128,255);
          break;
      }
      SDL_RenderFillRect(renderer,&(curr->rect));    
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
