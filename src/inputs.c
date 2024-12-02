#include "../include/inputs.h"
#include "../include/main.h"
#include "../include/initialize.h"

// Get the state of a switch
int get_sw(void) {
    volatile int* switch_adress = (volatile int*) 0x4000010; // Address for the switch input
    return *switch_adress & 0x1; // Return the state of the first switch (bit 0)
}

// Get the state of the restart button
int get_btn_restart(void) {
    volatile int *button_adress = (volatile int *)0x40000d0; // Address for the restart button
    return *button_adress & 0x1; // Return the state of the restart button (bit 0)
}

// Get the state of GPIO Button 1
int get_btn1(void) {
    volatile int *button_adress = (volatile int *)0x40000e0;
    return *button_adress & 0x1; // Return the state of Button 1 (bit 0)
}

// Get the state of GPIO Button 2
int get_btn2(void) {
    volatile int *button_adress = (volatile int *)0x40000e0;
    return *button_adress & 0x2; // Return the state of Button 2 (bit 1)
}

// Check and process inputs from buttons and switches
void check_inputs(void) {
    // Increment random numbers for generating randomness
    random_number1++;
    random_number2++;
    
    // Reset random_number1 if it exceeds its limit and increment random_number2
    if (random_number1 >= 134) {
        random_number1 = 1;
        random_number2++;
    }

    // Reset random_number2 if it exceeds its limit
    if (random_number2 >= 278) {
        random_number2 = 1;
    }

    // Restart button input
    if (get_btn_restart() && !(reset_button_pressed)) { // If restart button is pressed and wasn't pressed previously
        reset_button_pressed = 1; // Mark restart button as pressed
        gameover = 0; // Reset the game over flag
        init_snake(); // Reinitialize the snake to restart the game
    } else if (!(get_btn_restart())) {
        // If the restart button is not pressed
        reset_button_pressed = 0; // Clear the pressed flag
    }

    // Handle left turn input (Button 1)
    if (!get_btn1() && !(left_button_pressed)) {
        // If Button 1 is pressed and wasn't pressed previously
        left_button_pressed = 1; // Mark Button 1 as pressed
        new_direction = (direction + 3) % 4; // Rotate the snake 90° counterclockwise
    } else if (get_btn1()) {
        // If Button 1 is not pressed
        left_button_pressed = 0; // Clear the pressed flag
    }

    // Handle right turn input (Button 2)
    if (!get_btn2() && !(right_button_pressed)) {
        // If Button 2 is pressed and wasn't pressed previously
        right_button_pressed = 1; // Mark Button 2 as pressed
        new_direction = (direction + 1) % 4; // Rotate the snake 90° clockwise
    } else if (get_btn2()) {
        // If Button 2 is not pressed
        right_button_pressed = 0; // Clear the pressed flag
    }
}
