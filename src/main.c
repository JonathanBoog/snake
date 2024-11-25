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
char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;

int direction;
const int snakespeed = 2; // 2 per second
int dx = 0;
int dy = 0;

volatile char *VGA = (volatile char *)0x08000000; // Pekare till VGA-pixelbufferten
const int square_size = 20;                       // Storleken på varje ruta i pixlar
const int num_rows = 240 / square_size;           // Antal rader
const int num_cols = 320 / square_size;           // Antal kolumner

int snake[100][2];    // Maximum length of the snake is 100 segments
int snake_length = 3; // Start with a snake of 3 segments

const int snake_color = 0xfff;

void set_displays(int display_number, int value)
{
  volatile int *display_adress = (volatile int *)(0x04000050 + display_number * 16);
  switch (value)
  {
  case 0:
    *display_adress = 0b1000000;
    break;
  case 1:
    *display_adress = 0b1111001;
    break;
  case 2:
    *display_adress = 0b0100100;
    break;
  case 3:
    *display_adress = 0b0110000;
    break;
  case 4:
    *display_adress = 0b0011001;
    break;
  case 5:
    *display_adress = 0b0010010;
    break;
  case 6:
    *display_adress = 0b0000010;
    break;
  case 7:
    *display_adress = 0b1111000;
    break;
  case 8:
    *display_adress = 0b0000000;
    break;
  case 9:
    *display_adress = 0b0010000;
    break;
  default:
    *display_adress = 0x00;
  }
}

int get_sw(void)
{
  volatile int *switch_adress = (volatile int *)0x4000010;
  return *switch_adress & 0x3FF;
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

    if (!get_btn1())
    {
      direction = (direction + 1) % 4;
    }
    if (!get_btn2())
    {
      direction = (direction - 1) % 4;
    }

    if (10 / snakespeed <= timeoutcount)
    {
      timeoutcount = 0;
      update_snake();
    }
  }
}

void draw_board()
{
  // Rita ett schackbrädsliknande mönster med gröna och mörkgröna rutor
  for (unsigned int row = 0; row < num_rows; row++)
  {
    for (unsigned int col = 0; col < num_cols; col++)
    {
      // Välj färg baserat på rutans position
      char color = (row + col) % 2 == 0 ? 0x0A : 0x02; // Grön och mörkgrön färg
      for (unsigned int y = 0; y < square_size; y++)
      {
        for (unsigned int x = 0; x < square_size; x++)
        {
          // Beräkna pixelns position i VGA-bufferten
          unsigned int pixel_index = (row * square_size + y) * 320 + (col * square_size + x);
          VGA[pixel_index] = color;
        }
      }
    }
  }
}
draw_snake(int snakex, int snakey, int color)
{
  // Välj färg baserat på rutans position

  for (unsigned int y = 0; y < square_size; y++)
  {
    for (unsigned int x = 0; x < square_size; x++)
    {
      // Beräkna pixelns position i VGA-bufferten
      unsigned int pixel_index = (snakex * square_size + y) * 320 + (snakey * square_size + x);
      VGA[pixel_index] = color;
    }
  }
}

draw_box(int boxx, int boxy)
{
  // Välj färg baserat på rutans position

  char color = (boxx + boxy) % 2 == 0 ? 0x0A : 0x02; // Grön och mörkgrön färg
  for (unsigned int y = 0; y < square_size; y++)
  {
    for (unsigned int x = 0; x < square_size; x++)
    {
      // Beräkna pixelns position i VGA-bufferten
      unsigned int pixel_index = (boxx * square_size + y) * 320 + (boxy * square_size + x);
      VGA[pixel_index] = color;
    }
  }
}

update_snake()
{
  draw_box(snake[snake_length - 1][0], snake[snake_length - 1][1]);
  // Move body segments (each segment follows the one before it)
  for (int i = snake_length - 1; i > 0; i--)
  {
    snake[i][0] = snake[i - 1][0]; // Copy row from the segment before
    snake[i][1] = snake[i - 1][1]; // Copy column from the segment before
  }
  draw_snake(snake[0][0], snake[0][1], snake_color);
}

void init_snake()
{

  int start_row = num_rows / 2; // Middle row
  int start_col = num_cols / 2; // Middle column

  for (int i = 0; i < snake_length; i++)
  {
    snake[i][0] = start_row;     // All segments in the same row
    snake[i][1] = start_col - i; // Positioned sequentially in the column
  }
  direction = 0;
  draw_board();
  draw_snake(snake[0][0], snake[0][1], snake_color);
  draw_snake(snake[1][0], snake[1][1], snake_color);
  draw_snake(snake[2][0], snake[2][1], snake_color);
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  volatile int *button_adress1 = (volatile int *)0x040000e0;
  // volatile int* button_adress2 = (volatile unsigned short*);

  *(button_adress1 + 1) = 0x0; // Sets direction to input
  //*(button_adress2 + 1) = 0x0; // Sets direction to input

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

  while (1)
  {
  }
}
