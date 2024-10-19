#include "functions.h"

int main(void){
  
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  if (TTF_Init() == -1) { // this initialize the library :c
    printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
    return -1;
    }

  SDL_Rect bar = {320,460,40,10};
  Ball ball;
  ball.rect.x = 340;
  ball.rect.y = 455;
  ball.rect.w = 5;
  ball.rect.h = 5;
  ball.vel_x = -(1/sqrt(2))*5;
  ball.vel_y = -(1/sqrt(2))*5;

  SDL_Rect top_bar ={0,80,640,1};

  SDL_Surface* surface_message;
  SDL_Texture* message;  
  SDL_Color font_color = {255,255,255};

  Block* head = NULL;
  create_lvl1(&head);
  
  SDL_Event e;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WINDOW_W,WINDOW_H, 0, &window, &renderer); NULL;

  TTF_Font *font = TTF_OpenFont("font/Arcade.ttf",60); // is 0 if the font is not open
  if(!font){
    printf("Error al cargar la fuente: %s\n",TTF_GetError());
    return -1;
  }


  

  int running = 1;
  int game_start = 0;

  int pos_x_delta = 0;
  int pos_y_delta = 0;

  int mouse_pos_x,mouse_pos_y;

  int score = 0;
  char buf[10];

  int curr_lives = 5;
  int curr_level = 1;

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
      check_block_collition(&head,curr,&ball,&pos_x_delta,&pos_y_delta,&score);
    }

    if(ball.rect.y >= 480){
      ball.vel_x = -(1/sqrt(2))*5;
      ball.vel_y = -(1/sqrt(2))*5;
      curr_lives -= 1;
      game_start = 0;
    }

    if(curr_lives == 0 || (head == NULL && curr_level == 2)){
      deallocate(&head);
      score += curr_lives * 5000;
      int run_menu = 1;

      char top_score_buf[50];

      FILE *file_r = fopen("topscore.txt","r");
      while (fgets(top_score_buf, sizeof(top_score_buf), file_r) != NULL) {
      }
      int topscore = atoi(top_score_buf);
      fclose(file_r);
      
      if (topscore < score){
        FILE *file_w = fopen("topscore.txt","w");
        sprintf(buf, "%d\n",score);
        fputs(buf, file_w);
        fclose(file_w);
        topscore = score;
      }

      while(run_menu){
        while(SDL_PollEvent(&e)){
          if(e.type == SDL_QUIT){
            run_menu = 0;
            running = 0;
          }
          else if (e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_RETURN){
              run_menu = 0;
              score = 0;
              curr_lives = 5;
              curr_level = 1;
              create_lvl1(&head);
            }
          }
        }

        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        surface_message= TTF_RenderText_Solid(font,"Game Over",font_color);
        message = SDL_CreateTextureFromSurface(renderer, surface_message);
        SDL_Rect text_rect_game_over = {180,15,surface_message->w,surface_message->h};
        SDL_RenderCopy(renderer,message,NULL,&text_rect_game_over); // pass the direction of the text_rect (pointer)

        sprintf(buf, "%d", score);
        char score_message[50] = "Your Score : ";
        strcat(score_message, buf);
        surface_message= TTF_RenderText_Solid(font,score_message,font_color);
        message = SDL_CreateTextureFromSurface(renderer, surface_message);
        SDL_Rect score_text_rect = {15,90,surface_message->w,surface_message->h};
        SDL_RenderCopy(renderer,message,NULL,&score_text_rect); 

        sprintf(buf, "%d", topscore);
        char top_score_message[50] = "Top Score : ";
        strcat(top_score_message, buf);
        surface_message= TTF_RenderText_Solid(font,top_score_message,font_color);
        message = SDL_CreateTextureFromSurface(renderer, surface_message);
        SDL_Rect top_score_text_rect = {15,180,surface_message->w,surface_message->h};
        SDL_RenderCopy(renderer,message,NULL,&top_score_text_rect); 

        surface_message= TTF_RenderText_Solid(font,"Press ENTER to play",font_color);
        message = SDL_CreateTextureFromSurface(renderer, surface_message);
        SDL_Rect press_enter_rect = {15,270,surface_message->w,surface_message->h};
        SDL_RenderCopy(renderer,message,NULL,&press_enter_rect); 
        
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
        
        
      }
    }

    if(head == NULL){
      switch (curr_level) {
        case 1:
          curr_level += 1;
          create_lvl2(&head);
          game_start = 0;
          curr_lives += 1;
          break;
          
      }
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
    
    sprintf(buf,"%d",score);
    surface_message= TTF_RenderText_Solid(font, buf,font_color);
    message = SDL_CreateTextureFromSurface(renderer, surface_message);
    SDL_Rect text_rect = {15,15,surface_message->w,surface_message->h};
    SDL_RenderCopy(renderer,message,NULL,&text_rect); // pass the direction of the text_rect (pointer)
  
    sprintf(buf,"%d",curr_lives);
    surface_message= TTF_RenderText_Solid(font, buf,font_color);
    message = SDL_CreateTextureFromSurface(renderer, surface_message);
    SDL_Rect lives_rect = {600,15,surface_message->w,surface_message->h};
    SDL_RenderCopy(renderer,message,NULL,&lives_rect); // pass the direction of the text_rect (pointer)

    SDL_SetRenderDrawColor(renderer,0,0,255,255);
    SDL_RenderFillRect(renderer,&bar);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&top_bar);




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


  SDL_FreeSurface(surface_message);
  SDL_DestroyTexture(message);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
