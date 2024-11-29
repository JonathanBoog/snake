#include "../include/main.h"

int get_sw( void ){
  volatile int* switch_adress = (volatile int*) 0x4000010;
  return *switch_adress & 0x1;
}

int get_btn_restart(void){
  volatile int *button_adress = (volatile int *)0x40000d0;
  return *button_adress & 0x1;
}

// get btn 1
int get_btn1(void)
{
  volatile int *button_adress = (volatile int *)0x40000e0;
  return *button_adress & 0x1;
}
// get btn 2
int get_btn2(void)
{
  volatile int *button_adress = (volatile int *)0x40000e0;
  return *button_adress & 0x2;
}

void check_inputs(void){
  random_number1++;
  random_number2++;
  if (random_number1 >= 134)
  {
    random_number1 = 1;
    random_number2++;
  }
  if (random_number2 >= 278)
  {
    random_number2 = 1;
  }

  if (get_btn_restart() && !(reset_button_pressed))
  {
    reset_button_pressed = 1;
    gameover = 0;
    init_snake();
  } else if (!(get_btn_restart()))
  {
    reset_button_pressed = 0;
  }
  
    

  if (!get_btn1() && !(left_button_pressed))
  {
    left_button_pressed = 1;
    new_direction = (direction + 3) % 4; // Vänster (rotera 90° moturs)
  } else if (get_btn1())
  {
    left_button_pressed = 0;
  }

  if (!get_btn2() && !(right_button_pressed))
  {
    right_button_pressed = 1;
    new_direction = (direction + 1) % 4; // Höger (rotera 90° medurs)

  } else if (get_btn2())
  {
    right_button_pressed = 0;
  }
}