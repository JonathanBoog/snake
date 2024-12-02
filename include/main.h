#ifndef MAIN_H
#define MAIN_H



extern int timeoutcount;

extern int random_number1;
extern int random_number2;

extern volatile int direction;
extern volatile int new_direction;
extern const int snakespeed;

extern volatile char *VGA; 
extern const int square_size;  
extern const int num_rows;         
extern const int num_cols;         

extern int snake[192][2]; 
extern int snake_length; 
extern const int snake_offset;
extern const int food_offset;

extern const int green1;
extern const int green2;
extern const int snake_color;
extern const int food_color;


extern int food_x, food_y;
extern int has_eaten;
extern int current_score;
extern int highest_score;
extern int gameover;

extern int reset_button_pressed;
extern int left_button_pressed;
extern int right_button_pressed;


#endif