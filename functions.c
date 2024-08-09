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

    if((ball->rect.x + ball->rect.w) + ball->vel_x >= 640){
      *pos_x_delta = -(ball->rect.x + ball->rect.w + ball->vel_x -640);
      ball->vel_x = ball->vel_x * -1;
      return;

    }

    if(ball->rect.y + ball->vel_y <= 0){
      *pos_y_delta = -(ball->rect.y + ball->vel_y);
      ball->vel_y = ball->vel_y * -1;
      return;

    }
}

void check_bar_collition(Ball* ball, SDL_Rect bar, int* pos_x_delta, int* pos_y_delta){

  int next_frame_pos_x = ball->rect.x + ball->vel_x;
  int next_frame_pos_y = ball->rect.y + ball->vel_y;

  if(next_frame_pos_y >= bar.y && next_frame_pos_y <= bar.y + bar.h && next_frame_pos_x >= bar.x && next_frame_pos_x <= bar.x + bar.w){

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

    //apply rays here too
    *pos_y_delta = bar.y - (ball->rect.h + next_frame_pos_y);
    ball->vel_x = 0;
    ball->vel_y = -8;

    float t = (bar.x + bar.w -ball->rect.x)/(float)bar.w;
    float angle = (-M_PI/3)*t + (M_PI/3)*(1-t);
    rotate_vector(&(ball->vel_x),&(ball->vel_y), angle);
  }
}


void check_block_collition(Block* block, Ball* ball, int* pos_x_delta, int* pos_y_delta){

  int next_frame_pos_x = ball->rect.x + ball->vel_x;
  int next_frame_pos_y = ball->rect.y + ball->vel_y;

  if(next_frame_pos_y >= block->rect.y && next_frame_pos_y <= block->rect.y+ block->rect.h && next_frame_pos_x >= block->rect.x && next_frame_pos_x <= block->rect.x + block->rect.w){

    block->render_state = 0;
    
    if(ball->vel_x >= 0 && ball->vel_y <= 0){
      float lambda = (float)((ball->rect.x + ball->rect.w) - block->rect.x)/(-ball->vel_x);
      int ray_collition_y = ball->rect.y + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = block->rect.x + -(ball->rect.x + ball->rect.w + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
      }else{
        *pos_y_delta = -(block->rect.y + block->rect.h) + (ball->rect.y + ball->vel_y);
        ball->vel_y = ball->vel_y * -1;
      }
    }else if (ball->vel_x <= 0 && ball->vel_y <= 0) {
      float lambda = (float)(ball->rect.x - (block->rect.x + block->rect.w))/ (-ball->vel_x);
      int ray_collition_y = ball->rect.y + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = (block->rect.x + block->rect.w) - (ball->rect.x + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
      }else{
        *pos_y_delta = (block->rect.y + block->rect.h) - (ball->rect.y + ball->vel_y);
        ball->vel_y = ball->vel_y * -1;
      }
    }else if(ball->vel_x >= 0 && ball->vel_y >= 0){
      float lambda = (float)((ball->rect.x + ball->rect.w) - block->rect.x)/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = block->rect.x - (ball->rect.x + ball->rect.w + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
      }else{
        *pos_y_delta = block->rect.y - (ball->rect.y + ball->rect.h + ball->vel_y);
        ball->vel_y = ball->vel_y * -1;
      }
    }else if(ball->vel_x <= 0 && ball->vel_y >= 0){
      float lambda = (float)(ball->rect.x - (block->rect.x + block->rect.w))/ (-ball->vel_x);
      int ray_collition_y = (ball->rect.y + ball->rect.h) + lambda * ball->vel_y;
      if(ray_collition_y >= block->rect.y && ray_collition_y <= block->rect.y + block->rect.h){
        *pos_x_delta = (block->rect.x + block->rect.w) - (ball->rect.x + ball->vel_x);
        ball->vel_x = ball->vel_x * -1;
      }else{
        *pos_y_delta = block->rect.y - (ball->rect.y + ball->rect.h + ball->vel_y);
        ball->vel_y = ball->vel_y * -1;
      }
    }
  
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

