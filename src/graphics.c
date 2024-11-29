#include "../include/main.h"

//Draw a "chess"board
void draw_board(void)
{
  int color;
  for (unsigned int row = 0; row < num_rows; row++)
  {
    for (unsigned int col = 0; col < num_cols; col++)
    {
      color = get_correct_board_color(row, col);
      draw_box(row, col, color, 0, 0, 0, 0);
    }
  }
}
// Draws a square, rows & columns, color, and indent from left, top, right and bottom.
void draw_box(int boxx, int boxy, int color, int leftoffset, int topoffset, int rightoffset,int downoffset){
  unsigned int pixel_index;
  for (unsigned int y = topoffset; y < square_size-downoffset; y++){
    for (unsigned int x = leftoffset; x < square_size-rightoffset; x++){ // for (unsigned int x = timeoutcount/square_size; x < (timeoutcount+1)/square_size; x++)
      // Beräkna pixelns position i VGA-bufferten
      pixel_index = (boxx * square_size + y) * 320 + (boxy * square_size + x);
      VGA[pixel_index] = color;
    }
  }
}



void set_displays(int display_number, int value)
{
  volatile int *display_adress = (volatile int *)(0x04000050 + display_number * 16);
  switch (value)
  {
  case 0:
    *display_adress = 0b11000000;
    break;
  case 1:
    *display_adress = 0b11111001;
    break;
  case 2:
    *display_adress = 0b10100100;
    break;
  case 3:
    *display_adress = 0b10110000;
    break;
  case 4:
    *display_adress = 0b10011001;
    break;
  case 5:
    *display_adress = 0b10010010;
    break;
  case 6:
    *display_adress = 0b10000010;
    break;
  case 7:
    *display_adress = 0b11111000;
    break;
  case 8:
    *display_adress = 0b10000000;
    break;
  case 9:
    *display_adress = 0b10010000;
    break;
  //Bokstaven C
  case 10: 
    *display_adress = 0b11000110;
    break;
  //Bokstaven H
  case 11: 
    *display_adress = 0b10001001;
    break;
  default:
    *display_adress = 0xff;
  }
}

void display_score(int score, char letter){
 
  
  set_displays(0, score%10);
  set_displays(1, (score/10)%10);
  set_displays(2, score/100);
  set_displays(3, 12);
  set_displays(4, 12);

  if (letter == 'c')
  {
    set_displays(5, 10);
  } else if (letter == 'h'){
    set_displays(5, 11);
  }
}