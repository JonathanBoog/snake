#include "../include/main.h"

int check_collision(void){
  // Kontrollera väggkollision
  if ((snake[0][0] < 0) || (snake[0][0] >= num_rows) || (snake[0][1] < 0) || (snake[0][1] >= num_cols)){
    return 1; // Kollision
  }
  // Kontrollera kollision med kroppen
  for (int i = 1; i < snake_length; i++){
    if ((snake[0][0] == snake[i][0]) && (snake[0][1] == snake[i][1])){
      return 1; // Kollision
    }
  }
  return 0; // Ingen kollision
}

// get one of the green colors for the board, (the colors alternate every other)
int get_correct_board_color(int row, int col){
  return (row + col) % 2 == 0 ? green1 : green2;
}

// CHECK FOOD
void check_food_collision(void){
  if ((snake[0][0] == food_x) && (snake[0][1] == food_y)){
    has_eaten = 1;
    snake_length++; // Öka längden
    current_highscore++;
    spawn_food();   // Generera ny mat
  }
}

// FOOD
void spawn_food(void){
  int collision;
 

  
    food_x = random_range(random_number1, num_rows);
    food_y = random_range(random_number2, num_cols);

  do {
    // Kontrollera om maten krockar med ormens kropp
    collision = 0; // Förutsätt att det inte är en kollision
    for (int i = 0; i < snake_length; i++){
      if ((snake[i][0] == food_x) && (snake[i][1] == food_y)){
        collision = 1; // Kollision hittad
        
        if (food_y == (num_cols-1)){
          food_x = (food_x + 1) % num_rows;
          food_y = (food_y + 1) % num_cols;       
        } else{
        food_y = (food_y + 1) % num_cols;
        }
        if (current_highscore == (num_cols*num_rows-3))
        {
          collision = 0;
          game_over();
        }
      }
    }
  } while (collision); // Fortsätt om det finns en kollision

  // Rita maten på spelplanen
  if (current_highscore != (num_cols*num_rows-3))
  {
  draw_box(food_x, food_y, food_color, food_offset, food_offset, food_offset, food_offset);
  }
}


//UPDATE SNAKE
void update_snake(void){
  int tail_row = snake[snake_length - 1][0];
  int tail_col = snake[snake_length - 1][1];

  int left_offset1 = snake_offset;
  int top_offset1 = snake_offset;
  int right_offset1 = snake_offset;
  int down_offset1 = snake_offset;
  int left_offset2 = snake_offset;
  int top_offset2 = snake_offset;
  int right_offset2 = snake_offset;
  int down_offset2 = snake_offset;
  
  if (!has_eaten){
    draw_box(tail_row, tail_col, get_correct_board_color(tail_row, tail_col), 0, 0, 0, 0);
  } else{
    has_eaten = 0;
  }
  
  // Move body segments (each segment follows the one before it)
  for (int i = snake_length - 1; i > 0; i--)
  {
    snake[i][0] = snake[i - 1][0]; // Copy row from the segment before
    snake[i][1] = snake[i - 1][1]; // Copy column from the segment before
  }
  
  // Flytta ormens huvud
  switch (direction % 4)
  {
  case 0:
    snake[0][1]++; // Höger
    right_offset1 = 0;
    left_offset1 = square_size-snake_offset;
    left_offset2 = 0;
    break;
  case 1:
    snake[0][0]++; // Upp
    down_offset1 = 0;
    top_offset1 = square_size-snake_offset;
    top_offset2 = 0;
    
    break;
  case 2:
    snake[0][1]--; // Vänster
    left_offset1 = 0;
    right_offset1 = square_size-snake_offset;
    right_offset2 = 0;
    break;
  case 3:
    snake[0][0]--; // Ner
    top_offset1 = 0;
    down_offset1 = square_size-snake_offset;
    down_offset2 = 0;
    break;
  
  default:
    break;
  }

  if (check_collision())
    {
      game_over();
      return;
    }
  check_food_collision();


  draw_box(snake[1][0], snake[1][1], snake_color, left_offset1, top_offset1, right_offset1, down_offset1);
  draw_box(snake[0][0], snake[0][1], snake_color, left_offset2, top_offset2, right_offset2, down_offset2);
  
}

//END GAME ;(
void game_over(void){
  
  gameover = 1;
  // Set highscore
  if (current_highscore > highest_score) {
    highest_score = current_highscore;
  }
}