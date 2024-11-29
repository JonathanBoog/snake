/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */

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
const int square_size = 20;                       // Storleken på varje ruta i pixlar
const int num_rows = 240 / square_size;           // Antal rader
const int num_cols = 320 / square_size;           // Antal kolumner

int snake[192][2];    // Maximum length of the snake is 100 segments
int snake_length; // Start with a snake of 3 segments
const int snake_offset = 3;
const int food_offset = 5;

const int green1 = 0x4C;
const int green2 = 0x9C;
const int snake_color = 0xff;
const int head_color = 0x45;
const int food_color = 0xe0;

int food_x, food_y;
int has_eaten;
int current_highscore;
int highest_score = 0;
int gameover = 0;

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


// Funktion för att begränsa värdet till ett intervall [0, max - 1]
unsigned int random_range(int random_num, unsigned int max)
{
  return (433* random_num + 12345) % max;
}

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

//Draw a "chess"board
void draw_board(void)
{
  int color;
  for (unsigned int row = 0; row < num_rows; row++)
  {
    for (unsigned int col = 0; col < num_cols; col++)
    {
      color = (row + col) % 2 == 0 ? green1 : green2;
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



int check_collision(void){
  // Kontrollera väggkollision
  if ((snake[0][0] < 0) || (snake[0][0] >= num_rows) || (snake[0][1] < 0) || (snake[0][1] >= num_cols)){
    return 1; // Kollision
  }
  // Kontrollera kollision med kroppen
  for (int i = 1; i < snake_length; i++){
    if ((snake[0][0] == snake[i][0]) && (snake[0][1] == snake[i][1])){
      return 1; // Kollision
    }
  }
  return 0; // Ingen kollision
}

// FOOD
void spawn_food(void){
  int collision;
 

  
    food_x = random_range(random_number1, num_rows);
    food_y = random_range(random_number2, num_cols);

  do {
    // Kontrollera om maten krockar med ormens kropp
    collision = 0; // Förutsätt att det inte är en kollision
    for (int i = 0; i < snake_length; i++){
      if ((snake[i][0] == food_x) && (snake[i][1] == food_y)){
        collision = 1; // Kollision hittad
        
        if (food_y == (num_cols-1)){
          food_x = (food_x + 1) % num_rows;
          food_y = (food_y + 1) % num_cols;
          print_dec(food_x);
        print("-move X-");
          
        } else{
        food_y = (food_y + 1) % num_cols;
        print("-move Y-");
          print_dec(food_y);
        }
        if (current_highscore == (num_cols*num_rows-3))
        {
          collision = 0;
          game_over();
        }
        
        
      }
    }
  } while (collision); // Fortsätt om det finns en kollision

  // Rita maten på spelplanen
  if (current_highscore != (num_cols*num_rows-3))
    {
      draw_box(food_x, food_y, food_color, food_offset, food_offset, food_offset, food_offset);
    }
  

}

// CHECK FOOD
void check_food_collision(void){
  if ((snake[0][0] == food_x) && (snake[0][1] == food_y)){
    has_eaten = 1;
    snake_length++; // Öka längden
    current_highscore++;
    spawn_food();   // Generera ny mat
  }
}

//UPDATE SNAKE
void update_snake(void){
  int color = (snake[snake_length - 1][0] + snake[snake_length - 1][1]) % 2 == 0 ? green1 : green2;
  int left_offset1 = snake_offset;
  int top_offset1 = snake_offset;
  int right_offset1 = snake_offset;
  int down_offset1 = snake_offset;
  int left_offset2 = snake_offset;
  int top_offset2 = snake_offset;
  int right_offset2 = snake_offset;
  int down_offset2 = snake_offset;
  
  if (!has_eaten){
    draw_box(snake[snake_length - 1][0], snake[snake_length - 1][1], color, 0, 0, 0, 0);
  } else{
    has_eaten = 0;
  }
  
  // Move body segments (each segment follows the one before it)
  for (int i = snake_length - 1; i > 0; i--)
  {
    snake[i][0] = snake[i - 1][0]; // Copy row from the segment before
    snake[i][1] = snake[i - 1][1]; // Copy column from the segment before
  }
  // Flytta ormens huvud
  switch (direction % 4)
  {
  case 0:
    snake[0][1]++; // Höger
    right_offset1 = 0;
    left_offset1 = square_size-snake_offset;
    left_offset2 = 0;
    break;
  case 1:
    snake[0][0]++; // Upp
    down_offset1 = 0;
    top_offset1 = square_size-snake_offset;
    top_offset2 = 0;
    
    break;
  case 2:
    snake[0][1]--; // Vänster
    left_offset1 = 0;
    right_offset1 = square_size-snake_offset;
    right_offset2 = 0;
    break;
  case 3:
    snake[0][0]--; // Ner
    top_offset1 = 0;
    down_offset1 = square_size-snake_offset;
    down_offset2 = 0;
    break;
  
  default:
    break;
  }

  if (check_collision())
    {
      game_over();
      return;
    }
  check_food_collision();

  draw_box(snake[1][0], snake[1][1], snake_color, left_offset1, top_offset1, right_offset1, down_offset1);
  draw_box(snake[0][0], snake[0][1], snake_color, left_offset2, top_offset2, right_offset2, down_offset2);
}

//END GAME ;(
void game_over(void){
  
  gameover = 1;
  // Set highscore
  if (current_highscore > highest_score) {
    highest_score = current_highscore;
  }
}
void init_snake()
{
  snake_length = 3;
  direction = 0;
  new_direction = 0;
  has_eaten = 0;
  current_highscore = 0;

  int start_row = num_rows / 2; // Middle row
  int start_col = num_cols / 2; // Middle column

  for (int i = 0; i < snake_length; i++)
  {
    snake[i][0] = start_row;     // All segments in the same row
    snake[i][1] = start_col - i; // Positioned sequentially in the column
  }
  
  draw_board();
  draw_box(snake[0][0], snake[0][1], snake_color, 0, snake_offset, snake_offset, snake_offset);
  draw_box(snake[1][0], snake[1][1], snake_color, 0, snake_offset, 0, snake_offset);
  draw_box(snake[2][0], snake[2][1], snake_color, snake_offset, snake_offset, 0, snake_offset);
  spawn_food();
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  volatile int *button_adress = (volatile int *)0x040000e0;
  *(button_adress + 1) = 0x0; // Sets direction to input
 
  volatile unsigned short *timer_adress = (volatile unsigned short *)0x04000020;

  // Ställ in timeout-perioden till 3 000 000(-1,nollindexerat) cykler (100 ms vid 30 MHz)
  *(timer_adress + 4) = 2999999 & 0xFFFF;         // Lägsta 16 bitarna
  *(timer_adress + 6) = (2999999 >> 16) & 0xFFFF; // Högsta 16 bitarna

  // Starta timern
  *(timer_adress + 2) = 0x7; // START och CONT och ITO satt till 1

  asm volatile("csrsi mie,16");
  asm volatile("csrsi mstatus,3");
}

/* Your code goes into main as well as any needed functions. */
int main()
{
  // Call labinit()
  labinit();
  init_snake();

  int left_button_pressed = 0;
  int right_button_pressed = 0;


  while (1)
  {
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

    if (get_btn_restart())
    {
      gameover = 0;
      init_snake();
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
    delay(10);
  }
}