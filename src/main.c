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

int directions = 0;
const int snakespeed = 2; // 2 per second
int dx = 0;
int dy = 0;

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

void display_time(int mytime)
{
  int temp = mytime;

  set_displays(0, (temp & 0xf));

  set_displays(1, ((temp & 0xf0) >> 4));

  set_displays(2, ((temp & 0xf00) >> 8));

  set_displays(3, ((temp & 0xf000) >> 12));
  set_displays(4, hours);
  set_displays(5, tenth_hour);
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
      dx++;
      dy++;
      dx = dx % 2;
      dy = dy % 2;
    }
    if (!get_btn2())
    {
      dx--;
      dy--;
      dx = dx % 2;
      dy = dy % 2;
    }

    if(10/snakespeed == timeoutcount){
      timeoutcount = 0;
      draw(dx,dy);
    }
  }
}

void draw_board(){
  
}

void draw(dx,dy){

}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  volatile int *button_adress1 = (volatile int *)0x040000e0;
  // volatile int* button_adress2 = (volatile unsigned short*);

  *(button_adress1 + 1) = 0x0; // Sets direction to input

  // Test output: vad finns på addressen
  print_dec(*button_adress1);

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
  volatile char *VGA = (volatile char *)0x08000000; // Pekare till VGA-pixelbufferten
  unsigned int square_size = 20;                    // Storleken på varje ruta i pixlar
  unsigned int num_rows = 240 / square_size;        // Antal rader
  unsigned int num_cols = 320 / square_size;        // Antal kolumner

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

  while (1)
  {
  }
}
