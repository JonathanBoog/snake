/*
file to initialize the game as a whole, more specifically the snake and interrupts

Created by Jonathan Lavén & Julius Geiryd
2024-12-08
*/

#include "../include/game_logic.h"
#include "../include/main.h"

void init_snake(void)
{
  snake_length = 3;
  direction = 0;
  new_direction = 0;
  has_eaten = 0;
  current_score = 0;

  int start_row = num_rows / 2; // Middle row
  int start_col = num_cols / 2; // Middle column

  for (int i = 0; i < snake_length; i++)
  {
    snake[i][0] = start_row;     // All segments in the same row
    snake[i][1] = start_col - i; // Positioned sequentially in the column
  }
  
  draw_board();
  draw_box(snake[0][0], snake[0][1], snake_color, 0, snake_offset, snake_offset, snake_offset);
  draw_box(snake[1][0], snake[1][1], snake_color, 0, snake_offset, 0, snake_offset);
  draw_box(snake[2][0], snake[2][1], snake_color, snake_offset, snake_offset, 0, snake_offset);
  spawn_food();
}

/* Code to initialize interupt and also GPIO pin. */
void init(void)
{
  volatile int *button_adress = (volatile int *)0x040000e0;
  *(button_adress + 1) = 0x0; // Sets direction to input
 
  volatile unsigned short *timer_adress = (volatile unsigned short *)0x04000020;

  // Ställ in timeout-perioden till 3 000 000(-1,nollindexerat) cykler (100 ms vid 30 MHz)
  *(timer_adress + 4) = 2999999 & 0xFFFF;         // Lägsta 16 bitarna
  *(timer_adress + 6) = (2999999 >> 16) & 0xFFFF; // Högsta 16 bitarna

  // Starta timern
  *(timer_adress + 2) = 0x7; // START och CONT och ITO satt till 1

  asm volatile("csrsi mie,16");
  asm volatile("csrsi mstatus,3");
}