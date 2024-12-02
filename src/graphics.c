/*
Graphics file for the game, mostly VGA stuff and 7-segment display (output)

Created by Jonathan Lavén & Julius Geiryd
2024-12-08
*/

#include "../include/main.h"
#include "../include/graphics.h"

// Draw a "chess" board on the screen by alternating colors
void draw_board(void)
{
  int color;
  for (unsigned int row = 0; row < num_rows; row++) // Loop through rows of the board
  {
    for (unsigned int col = 0; col < num_cols; col++) // Loop through columns of the board
    {
      color = get_correct_board_color(row, col);  // Get correct color for the current position (row, col) based on pattern
      draw_box(row, col, color, 0, 0, 0, 0); // Draw a square
    }
  }
}

// Draws a square at the given position (boxx, boxy) with a specified color and optional offsets
void draw_box(int boxx, int boxy, int color, int leftoffset, int topoffset, int rightoffset, int downoffset)
{
  unsigned int pixel_index;
  for (unsigned int y = topoffset; y < square_size - downoffset; y++) // Loop through y-coordinate (vertical)
  {
    for (unsigned int x = leftoffset; x < square_size - rightoffset; x++) // Loop through x-coordinate (horizontal)
    {
      pixel_index = (boxx * square_size + y) * 320 + (boxy * square_size + x); //  the pixel index in the VGA buffer (320 is the width of the screen)
      VGA[pixel_index] = color; // Set the pixel color in the VGA buffer
    }
  }
}

// Set the value to be displayed on a 7-segment display
void set_displays(int display_number, int value)
{
  volatile int *display_adress = (volatile int *)(0x04000050 + display_number * 16); // Get the address of the display
  
  // Set the corresponding 7-segment display pattern based on the value
  switch (value)
  {
  case 0:
    *display_adress = 0b11000000; // Display 0
    break;
  case 1:
    *display_adress = 0b11111001; // Display 1
    break;
  case 2:
    *display_adress = 0b10100100; // Display 2
    break;
  case 3:
    *display_adress = 0b10110000; // Display 3
    break;
  case 4:
    *display_adress = 0b10011001; // Display 4
    break;
  case 5:
    *display_adress = 0b10010010; // Display 5
    break;
  case 6:
    *display_adress = 0b10000010; // Display 6
    break;
  case 7:
    *display_adress = 0b11111000; // Display 7
    break;
  case 8:
    *display_adress = 0b10000000; // Display 8
    break;
  case 9:
    *display_adress = 0b10010000; // Display 9
    break;
  case 10:
    *display_adress = 0b11000110; // Display C
    break;
  case 11:
    *display_adress = 0b10001001; // Display H
    break;
  default:
    *display_adress = 0xff; // Display all segments as off (invalid value)
  }
}

// Display the current score and a letter (either 'C' or 'H') on the 7-segment displays
void display_score(int score, char letter)
{
  // Display the hundreds, tens, and ones digits of the score on the first three displays
  set_displays(0, score % 10);     // Ones place
  set_displays(1, (score / 10) % 10); // Tens place
  set_displays(2, score / 100);      // Hundreds place

  set_displays(3, 12); // Empty display (12 is arbitrary, everything after 11 is ok to use)
  set_displays(4, 12); // Empty display

  if (letter == 'c') // Current score
  {
    set_displays(5, 10); // Display 'C'
  }
  else if (letter == 'h') // Highest score
  {
    set_displays(5, 11); // Display 'H'
  }
}
