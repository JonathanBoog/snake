/*
file to initialize the game as a whole, more specifically the snake and interrupts

Created by Jonathan Lavén & Julius Geiryd
2024-12-08
*/

#include "../include/game_logic.h"
#include "../include/graphics.h"

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


// Initialize the snake at the start of the game or after a reset
void init_snake(void) {
    snake_length = 3;    // Start with a snake of length 3
    direction = 0;       // Initial direction (right)
    new_direction = 0;   // New direction (aligned with the initial direction)
    has_eaten = 0;       // Snake has not eaten any food yet
    current_score = 0;   // Reset the score to 0

    // Calculate the starting position (middle of the board)
    int start_row = num_rows / 2; 
    int start_col = num_cols / 2;

    // Initialize the snake's body
    for (int i = 0; i < snake_length; i++) {
        snake[i][0] = start_row;     // All segments in the same row
        snake[i][1] = start_col - i; // Positioned sequentially in columns to form the initial snake
    }

    draw_board(); // Draw the game board

    // Draw each segment of the snake with appropriate colors and offsets
    draw_box(snake[0][0], snake[0][1], snake_color, 0, snake_offset, snake_offset, snake_offset); // Head
    draw_box(snake[1][0], snake[1][1], snake_color, 0, snake_offset, 0, snake_offset); // Middle segment
    draw_box(snake[2][0], snake[2][1], snake_color, snake_offset, snake_offset, 0, snake_offset); // Tail

    
    spawn_food(); // Spawn the first piece of food on the board
}

// Function to initialize interrupts and timer
void init(void) {
    volatile int *button_adress = (volatile int *)0x040000e0; // Address for button control
    *(button_adress + 1) = 0x0; // Set button direction to input

    volatile unsigned short *timer_adress = (volatile unsigned short *)0x04000020; // Address for timer control

    // Set timeout period to 3,000,000 cycles (100 ms at 30 MHz)
    *(timer_adress + 4) = 2999999 & 0xFFFF;         // Set the lower 16 bits of the timeout value
    *(timer_adress + 6) = (2999999 >> 16) & 0xFFFF; // Set the upper 16 bits of the timeout value

    // Start the timer with START, CONT, and ITO bits set
    *(timer_adress + 2) = 0x7; 

    // Enable machine-level interrupts
    asm volatile("csrsi mie,16"); // Enable external interrupts in the MIE (Machine Interrupt Enable) CSR
    asm volatile("csrsi mstatus,3"); // Enable global interrupts and set the processor mode to Machine mode
}
