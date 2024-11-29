#ifndef MAIN_H
#define MAIN_H



extern int timeoutcount;

extern int random_number1;
extern int random_number2;

extern volatile int direction;
extern volatile int new_direction;
extern const int snakespeed;

extern volatile char *VGA; // Pekare till VGA-pixelbufferten
extern const int square_size;                       // Storleken på varje ruta i pixlar
extern const int num_rows;           // Antal rader
extern const int num_cols;           // Antal kolumner

extern int snake[192][2];    // Maximum length of the snake is 100 segments
extern int snake_length; // Start with a snake of 3 segments
extern const int snake_offset;
extern const int food_offset;

extern const int green1;
extern const int green2;
extern const int snake_color;
extern const int food_color;


extern int food_x, food_y;
extern int has_eaten;
extern int current_highscore;
extern int highest_score;
extern int gameover;

extern int reset_button_pressed;
extern int left_button_pressed;
extern int right_button_pressed;


#endif