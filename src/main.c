/*
The main file to run snake.

Created by Jonathan Lavén & Julius Geiryd
2024-12-08
*/

#include "../include/game_logic.h"
#include "../include/initialize.h"
#include "../include/graphics.h"
#include "../include/inputs.h"
#include "../include/analyze.h"

// External functions declarations from other modules
extern void print(const char *);               // Print a string to console or log
extern void print_dec(unsigned int);
extern void delay(int);                        // Create a delay in execution

/* 
 * Interrupt handler function, called when an interrupt occurs.
 * Handles timer interrupts, score updates, and snake updates.
 */
void handle_interrupt(unsigned cause) {
  

  volatile unsigned short *timer_adress = (volatile unsigned short *)0x04000020;

  if (cause == 16) { // Timer interrupt
    timeoutcount++;
    *(timer_adress) &= ~0x1;

    // Update highest score if current highscore exceeds it
    if (current_score > highest_score) {
      highest_score = current_score;
    }

    // Display the appropriate score based on a switch state
    if (get_sw()) {
      display_score(current_score, 'c'); // Display current score
    } else {
      display_score(highest_score, 'h');    // Display highest score
    }

    // Move the snake
    if ((10 / snakespeed <= timeoutcount) && !gameover) {
      clear_counters();
      timeoutcount = 0;

      // Update direction if the new direction is not opposite to the current one
      if (new_direction != (direction + 2) % 4) {
        direction = new_direction; // Update the snake's direction
      }

      update_snake(); // Update the snake's position
      read_counters();
    }
  }
  
}

/* 
 * Main function.
 */
int main() {
  init();      // Initialize hardware or peripherals
  init_snake();   // Initialize the snake's initial state

  // Main game loop
  while (1) {
    check_inputs(); 
    delay(10);      
  }
}
