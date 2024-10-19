#include "functions.h"

void rotate_vector(float* x,float* y,float deg){
  
  double rotation_matrix[2][2] = { {cos(deg),-sin(deg)}, {sin(deg),cos(deg)}};

  float vec_x = *x;
  float vec_y = *y;

  *x = rotation_matrix[0][0] * vec_x + rotation_matrix[0][1] * vec_y;
  *y = rotation_matrix[1][0] * vec_x + rotation_matrix[1][1] * vec_y;

}

void check_border_collition(Ball* ball, int* pos_x_delta, int* pos_y_delta){

    if(ball->rect.x + ball->vel_x <= 0){
      *pos_x_delta = -(ball->rect.x + ball->vel_x);
      ball->vel_x = ball->vel_x * -1;
      return;
    }

    if((ball->rect.x + ball->rect.w) + ball->vel_x >= WINDOW_W){
      *pos_x_delta = -(ball->rect.x + ball->rect.w + ball->vel_x -WINDOW_W);
      ball->vel_x = ball->vel_x * -1;
      return;

    }

    if(ball->rect.y + ball->vel_y <= 80){
      *pos_y_delta = -(ball->rect.y + ball->vel_y - 80);
      ball->vel_y = ball->vel_y * -1;
      return;

    }
}

void check_bar_collition(Ball* ball, SDL_Rect bar, int* pos_x_delta, int* pos_y_delta){

  int next_frame_pos_x = ball->rect.x + ball->vel_x;
  int next_frame_pos_y = ball->rect.y + ball->vel_y;

  if(next_frame_pos_y + ball->rect.h >= bar.y && next_frame_pos_x >= bar.x && next_frame_pos_x <= bar.x + bar.w){

    if(ball->vel_x >= 0 && ball->vel_y >= 0){
      float lambda = (float)((ball->rect.x + ball->rect.w) - bar.x)/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= bar.y && ray_collition_y <= bar.y + bar.h){
        *pos_x_delta = bar.x - (ball->rect.x + ball->rect.w + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
        return;
      }
    }
    if(ball->vel_x <= 0 && ball->vel_y >= 0){
      float lambda = (float)(ball->rect.x - (bar.x + bar.w))/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= bar.y && ray_collition_y <= bar.y + bar.h){
        *pos_x_delta = (bar.x + bar.w) - (ball->rect.x + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
        return;
      }
    }

    //uses a ray to calculate te collition with the bar also uses a linear combination to change the direction of the ball :c
    float lambda = (float)((ball->rect.y + ball->rect.h) - bar.y)/(-ball->vel_y);
    int ray_collition_x = ball->rect.x + lambda * ball->vel_x;

    float current_vel_mod = sqrt(pow(ball->vel_x,2) + pow(ball->vel_y,2)); 
    ball->vel_x = 0;
    ball->vel_y = current_vel_mod * -1;

    float t = (bar.x + bar.w -ray_collition_x)/(float)bar.w;
    float angle = (-M_PI/(float)3)*t + (M_PI/(float)3)*(1-t);
    rotate_vector(&(ball->vel_x),&(ball->vel_y), angle);

    float factor = sqrt(pow(ray_collition_x - next_frame_pos_x, 2) + pow(bar.y - next_frame_pos_y, 2));
    *pos_x_delta = (ball->vel_x/current_vel_mod)*factor;
    *pos_y_delta = (ball->vel_y/current_vel_mod)*factor;

    ball->rect.x = ray_collition_x;
    ball->rect.y = bar.y - ball->rect.h;
  }
}


void check_block_collition(Block** head, Block* block, Ball* ball, int* pos_x_delta, int* pos_y_delta, int* score){

  int next_frame_pos_x = ball->rect.x + ball->vel_x;
  int next_frame_pos_y = ball->rect.y + ball->vel_y;

  if(next_frame_pos_y + ball->rect.h >= block->rect.y && next_frame_pos_y <= block->rect.y+ block->rect.h && next_frame_pos_x + ball->rect.w >= block->rect.x && next_frame_pos_x <= block->rect.x + block->rect.w){


    if(ball->vel_x >= 0 && ball->vel_y <= 0){
      float lambda = (float)((ball->rect.x + ball->rect.w) - block->rect.x)/(-ball->vel_x);
      int ray_collition_y = ball->rect.y + lambda * ball->vel_y;
      if(ray_collition_y + ball->rect.h >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = block->rect.x + -(ball->rect.x + ball->rect.w + ball->vel_x);
        ball->vel_x *= -1;
      }else{
        *pos_y_delta = (block->rect.y + block->rect.h) - (ball->rect.y + ball->vel_y);
        ball->vel_y *= -1;
      }
    }else if (ball->vel_x <= 0 && ball->vel_y <= 0) {
      float lambda = (float)(ball->rect.x - (block->rect.x + block->rect.w))/ (-ball->vel_x);
      int ray_collition_y = ball->rect.y + lambda * ball->vel_y;
      if(ray_collition_y + ball->rect.h >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = (block->rect.x + block->rect.w) - (ball->rect.x + ball->vel_x);
        ball->vel_x *= -1;
      }else{
        *pos_y_delta = (block->rect.y + block->rect.h) - (ball->rect.y + ball->vel_y);
        ball->vel_y *= -1;
      }
    }else if(ball->vel_x >= 0 && ball->vel_y >= 0){
      float lambda = (float)((ball->rect.x + ball->rect.w) - block->rect.x)/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y - ball->rect.h <= block->rect.y + block->rect.h){
        *pos_x_delta = block->rect.x - (ball->rect.x + ball->rect.w + ball->vel_x);
        ball->vel_x *= -1;
      }else{
        *pos_y_delta = block->rect.y - (ball->rect.y + ball->rect.h + ball->vel_y);
        ball->vel_y *= -1;
      }
    }else if(ball->vel_x <= 0 && ball->vel_y >= 0){
      float lambda = (float)(ball->rect.x - (block->rect.x + block->rect.w))/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y - ball->rect.h <= block->rect.y + block->rect.h){
        *pos_x_delta = (block->rect.x + block->rect.w) - (ball->rect.x + ball->vel_x);
        ball->vel_x *= -1;
      }else{
        *pos_y_delta = block->rect.y - (ball->rect.y + ball->rect.h + ball->vel_y);
        ball->vel_y *= -1;
      }

    }
    float current_vel_mod = sqrt(pow(ball->vel_x,2) + pow(ball->vel_y,2)); 
    *score += 5*current_vel_mod;

    if (current_vel_mod <= 10){
      ball->vel_x *= 1.1;
      ball->vel_y *= 1.1;
    } 
    delete_block(head, block->rect.x, block->rect.y);
  
  }
   
}
void deallocate(Block** head){
  if(*head == NULL){
    return;
  }

  Block* curr = *head;
  while(curr != NULL){
    Block* aux = curr;
    curr = curr->next;
    free(aux);
  }

  *head = NULL;
}

void delete_block(Block** head, int pos_x, int pos_y){
  
  Block* curr = *head;
  if(curr->rect.x == pos_x && curr->rect.y == pos_y){
    *head = curr->next;
    free(curr);
    return;
  }

  while(curr->next != NULL){
    if(curr->next->rect.x == pos_x && curr->next->rect.y == pos_y){
      Block* aux = curr->next;
      curr->next = curr->next->next;
      free(aux);
      return;
    }
    curr = curr->next;
  }

}


void create_lvl1(Block** head){
  *head = malloc(sizeof(Block));
  Block* curr = *head;
  curr->rect.x = WINDOW_W - 120;
  curr->rect.y = WINDOW_H - 240;
  curr->rect.w = 40;
  curr->rect.h = 15;
  curr->color = RED; 
  curr->next = NULL;

  int pos_x = curr->rect.x;
  int pos_y = curr->rect.y - curr->rect.h - 3 ;
  int current_color = 2;

  while(pos_x - 60 > 0){
      
    while(pos_y - 100  > 0){

      curr->next = malloc(sizeof(Block));
      curr = curr->next;

      curr->rect.x = pos_x;
      curr->rect.y = pos_y;
      curr->rect.w = 40;
      curr->rect.h = 15;
      curr->next = NULL;

      switch (current_color) {
        case 1:
          curr->color = RED;
          current_color = 2;
          break;
        case 2:
          curr->color = GREEN;
          current_color = 3;
          break;
        case 3:
          curr->color = YELLOW;
          current_color = 4;
          break;
        case 4:
          curr->color = PURPLE;
          current_color = 1;
          break;
      }

      pos_y = curr->rect.y - curr->rect.h - 3;

      
    }

    pos_x = curr->rect.x - (curr->rect.w + 3);
    pos_y = WINDOW_H - 240; 
  }
}

void create_lvl2(Block** head){
  *head = malloc(sizeof(Block));
  Block* curr = *head;
  curr->rect.x = WINDOW_W - 43;
  curr->rect.y = WINDOW_H - 120;
  curr->rect.w = 40;
  curr->rect.h = 15;
  curr->color = RED; 
  curr->next = NULL;

  int pos_x = curr->rect.x;
  int pos_y = curr->rect.y - curr->rect.h - 3 ;
  int current_color = 2;
  int count_rows = 0;

  while(pos_x - 3 > 0){
      
    while(pos_y - 100 > 0){

      curr->next = malloc(sizeof(Block));
      curr = curr->next;

      curr->rect.x = pos_x;
      curr->rect.y = pos_y;
      curr->rect.w = 40;
      curr->rect.h = 15;
      curr->next = NULL;

      switch (current_color) {
        case 1:
          curr->color = RED;
          current_color = 2;
          break;
        case 2:
          curr->color = GREEN;
          current_color = 3;
          break;
        case 3:
          curr->color = YELLOW;
          current_color = 4;
          break;
        case 4:
          curr->color = PURPLE;
          current_color = 1;
          break;
      }

      pos_y = curr->rect.y - curr->rect.h - 3;

      
    }
    if(count_rows < 15){
      count_rows++;
    }

    pos_x = curr->rect.x - (curr->rect.w + 3);
    pos_y = WINDOW_H - 120 -(count_rows * (curr->rect.h+3));
  }
}
/* Si Hago lo siguiente piensa que estoy dereferenciando el puntero

pos_y = WINDOW_H - 240 -(count_rows*(curr->rect.w+3)) 

me entrega creo que la direccion de memoria de la wea xd
 
*/





