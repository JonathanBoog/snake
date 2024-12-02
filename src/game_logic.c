/*
File for the logic behind the game. collision etc..

Created by Jonathan Lavén & Julius Geiryd
2024-12-08
*/

#include "../include/main.h"
#include "../include/game_logic.h"

// Function to check if the snake collides with walls or itself
int check_collision(void){
  // Check wall collision
  if ((snake[0][0] < 0) || (snake[0][0] >= num_rows) || (snake[0][1] < 0) || (snake[0][1] >= num_cols)){
    return 1; // Collision detected
  }
  // Check collision with snake's body
  for (int i = 1; i < snake_length; i++){
    if ((snake[0][0] == snake[i][0]) && (snake[0][1] == snake[i][1])){
      return 1; // Collision detected
    }
  }
  return 0; // No collision
}

// Function to get the appropriate board color for a cell (alternates based on position)
int get_correct_board_color(int row, int col){
  return (row + col) % 2 == 0 ? green1 : green2; // Alternate colors for a checkerboard pattern
}

// Function to check if the snake eats food
void check_food_collision(void){
  if ((snake[0][0] == food_x) && (snake[0][1] == food_y)){
    has_eaten = 1;  // Mark that the snake has eaten food
    snake_length++; // Increase snake's length
    current_score++; // Increase score
    spawn_food();   // Spawn new food
  }
}

// Function to spawn food at a random position, avoiding collisions with the snake
void spawn_food(void){
  int collision;
   
   // Generate initial food coordinates
    food_x = random_range(random_number1, num_rows);
    food_y = random_range(random_number2, num_cols);

// Ensure the food does not overlap with the snake
  do {
    collision = 0; // Assume no collision initially
    for (int i = 0; i < snake_length; i++){
      if ((snake[i][0] == food_x) && (snake[i][1] == food_y)){
        collision = 1; // Collision detected
        
        // Adjust food position
        if (food_y == (num_cols-1)){
          food_x = (food_x + 1) % num_rows;
          food_y = (food_y + 1) % num_cols;       
        } else{
        food_y = (food_y + 1) % num_cols;
        }
        // Check if the board is completely filled
        if (current_score == (num_cols*num_rows-3))
        {
          collision = 0;
          game_over(); // End the game if the board is full
        }
      }
    }
  } while (collision); // Repeat if there's still a collision

  // Draw the food on the game board if board not full
  if (current_score != (num_cols*num_rows-3))
  {
  draw_box(food_x, food_y, food_color, food_offset, food_offset, food_offset, food_offset);
  }
}

// Function to update the snake's position and handle gameplay mechanics
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
  
  // Erase the tail if the snake hasn't eaten
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
  
  // Move the snake's head based on the current direction
  switch (direction % 4)
  {
  case 0:
    snake[0][1]++; // Right
    right_offset1 = 0;
    left_offset1 = square_size-snake_offset;
    left_offset2 = 0;
    break;
  case 1:
    snake[0][0]++; // Up
    down_offset1 = 0;
    top_offset1 = square_size-snake_offset;
    top_offset2 = 0;
    
    break;
  case 2:
    snake[0][1]--; // Left
    left_offset1 = 0;
    right_offset1 = square_size-snake_offset;
    right_offset2 = 0;
    break;
  case 3:
    snake[0][0]--; // Down
    top_offset1 = 0;
    down_offset1 = square_size-snake_offset;
    down_offset2 = 0;
    break;
  
  default:
    break;
  }

// Check for collisions
  if (check_collision())
    {
      game_over(); // End the game if there's a collision
      return;
    }
  // Check if the snake eats food
  check_food_collision();

  // Draw the snake's body and head
  draw_box(snake[1][0], snake[1][1], snake_color, left_offset1, top_offset1, right_offset1, down_offset1);
  draw_box(snake[0][0], snake[0][1], snake_color, left_offset2, top_offset2, right_offset2, down_offset2);
  
}

// Function to handle game-over scenarios
void game_over(void){
  
  gameover = 1;       // Set the game-over flag
  draw_board();       // Redraw the game board
  // Update the high score if the current score exceeds it
  if (current_score > highest_score) {
    highest_score = current_score;
  }
}