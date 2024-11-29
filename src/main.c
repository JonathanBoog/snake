/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */

#include "../include/graphics.h"
#include "../include/inputs.h"
#include "../include/game_logic.h"
#include "../include/initialize.h"

extern void print(const char *);
extern void print_dec(unsigned int);
extern void display_string(char *);
extern void time2string(char *, int);
extern void tick(int *);
extern void delay(int);

int mytime = 0x5950;
int hours = 3;
int tenth_hour = 2;
int timeoutcount = 1;

int random_number1 = 1;
int random_number2 = 6;

volatile int direction;
volatile int new_direction;
const int snakespeed = 2;

volatile char *VGA = (volatile char *)0x08000000; // Pekare till VGA-pixelbufferten
const int square_size = 40;                       // Storleken på varje ruta i pixlar
const int num_rows = 240 / square_size;           // Antal rader
const int num_cols = 320 / square_size;           // Antal kolumner

int snake[192][2];    // Maximum length of the snake is 100 segments
int snake_length; // Start with a snake of 3 segments
const int snake_offset = square_size/5;
const int food_offset = square_size/5+2;

const int green1 = 0x4C;
const int green2 = 0x9C;
const int snake_color = 0xff;
const int food_color = 0xe0;


int food_x, food_y;
int has_eaten;
int current_highscore;
int highest_score = 0;
int gameover = 0;

int left_button_pressed = 0;
int right_button_pressed = 0;


// Funktion för att begränsa värdet till ett intervall [0, max - 1]
unsigned int random_range(int random_num, unsigned int max)
{
  return (433* random_num + 12345) % max;
}


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause)
{
  volatile unsigned short *timer_adress = (volatile unsigned short *)0x04000020;

  if (cause == 16)
  { // timer interrupt
    timeoutcount++;
    *(timer_adress) &= ~0x1;

    // highscore
    if (current_highscore > highest_score) {
    highest_score = current_highscore;
    }

    if(get_sw()){
      display_score(current_highscore, 'c');
    } else{
      display_score(highest_score, 'h');
    }
    //eventuellt kolla om har käkat -> långsamt plocka bort stjärten
    if ((10 / snakespeed <= timeoutcount) && !gameover)
    {
      timeoutcount = 0;
      
      if (new_direction != (direction + 2) % 4) // Tror eventuellt att if-satsen inte behövs
      {
        direction = new_direction; // Uppdatera riktningen
      }
      
      update_snake();
    }
    // eventuellt ha en draw huvud här
  }
}

/* Your code goes into main as well as any needed functions. */
int main()
{
  // Call labinit()
  labinit();
  init_snake();

  while (1)
  {
    check_inputs();
    delay(10);
  }
}