#include "../include/graphics.h"
#include "../include/inputs.h"
#include "../include/game_logic.h"
#include "../include/initialize.h"

// External functions declarations from other modules
extern void print(const char *);               // Print a string to console or log
extern void delay(int);                        // Create a delay in execution

// Variable related to time and gameplay
int timeoutcount = 1;                          // Tracks the number of timeout events

// Random number generation variables
int random_number1 = 1;                  
int random_number2 = 6;                        


volatile int direction;                        // Current direction of the snake
volatile int new_direction;                    
const int snakespeed = 5;                      // Speed of the snake

// VGA configuration and rendering settings
volatile char *VGA = (volatile char *)0x08000000; // Pointer to the VGA pixel buffer
const int square_size = 20;                       // Size of each square in pixels
const int num_rows = 240 / square_size;    
const int num_cols = 320 / square_size;    

// Snake-related variables
int snake[192][2];    // Array to store the snake's body coordinates (maximum 192 segments)
int snake_length;     // Current length of the snake
const int snake_offset = square_size / 5;    // Visual offset for rendering the snake
const int food_offset = square_size / 5 + 2; // Visual offset for rendering the food

// Color codes for graphics (format depends on the system)
const int green1 = 0x4C;   
const int green2 = 0x9C; 
const int snake_color = 0xff;
const int food_color = 0xe0; 

// Game state variables
int food_x, food_y;               // Coordinates of the food
int has_eaten;                    // Flag to check if the snake has eaten food
int current_score;            // Current score
int highest_score = 0;            // Highest score achieved in the session
int gameover = 0;                 // Game over flag

// Button state variables
int reset_button_pressed = 0;     // Flag for the reset button
int left_button_pressed = 0;      // Flag for the left button
int right_button_pressed = 0;     // Flag for the right button

// Function to generate a random number within the range [0, max - 1]
unsigned int random_range(int random_num, unsigned int max) {
  return (433 * random_num + 12345) % max; // Linear congruential generator formula
}

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
      timeoutcount = 0;

      // Update direction if the new direction is not opposite to the current one
      if (new_direction != (direction + 2) % 4) {
        direction = new_direction; // Update the snake's direction
      }

      update_snake(); // Update the snake's position
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
