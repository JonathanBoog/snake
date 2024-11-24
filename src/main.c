/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);


int mytime = 0x5950;
int hours = 3;
int tenth_hour = 2;
char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;

//Del 1e
void set_displays(int display_number, int value){
  volatile int* display_adress = (volatile int*) (0x04000050 + display_number*16);
  switch (value)
  {
  case 0: *display_adress = 0b1000000;
    break;
  case 1: *display_adress = 0b1111001;
    break;
  case 2: *display_adress = 0b0100100;
    break;
  case 3: *display_adress = 0b0110000;
    break;
  case 4: *display_adress = 0b0011001;
    break;
  case 5: *display_adress = 0b0010010;
    break;
  case 6: *display_adress = 0b0000010;
    break;
  case 7: *display_adress = 0b1111000;
    break;
  case 8: *display_adress = 0b0000000;
    break;
  case 9: *display_adress = 0b0010000;
    break;
  default:
     *display_adress = 0x00;
  }
}

//Del 1f
int get_sw( void ){
  volatile int* switch_adress = (volatile int*) 0x4000010;
  return *switch_adress & 0x3FF;
}

//Del 1g
int get_btn(void){
  volatile int* button_adress = (volatile int*) 0x40000e0;
  return *button_adress & 0x1;
}

void display_time(int mytime){
  int temp = mytime;
 
  set_displays(0, (temp & 0xf));
  
  set_displays(1, ((temp & 0xf0) >> 4));
  
  set_displays(2, ((temp & 0xf00) >> 8));

  set_displays(3, ((temp & 0xf000) >> 12));
  set_displays(4, hours);
  set_displays(5, tenth_hour);
}


/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {

    

  int state_swithes;
    volatile unsigned short* timer_adress = (volatile unsigned short*) 0x04000020;
    if(!get_btn()){
      state_swithes = get_sw();
      int new_time = state_swithes & 0b111111;
      int mode = (state_swithes & 0b1100000000) >> 8;
      int modnewtime = (new_time%10) + (new_time/10)*0x10;
      

    switch (mode){
      case 1:
        if (new_time < 60) { 
          mytime = (mytime & 0xFF00) | modnewtime;
        }
          break;
      case 2:
        if (new_time < 60) { 
          mytime = (mytime & 0x00FF) | (modnewtime << 8);
        }
        break;
      case 3:
        if (new_time < 24) { 
          hours = (new_time%10);
          tenth_hour = (new_time/10);
        }

      default:
        break;
      }   
    }

    if (cause == 16){ //timer interrupt
      timeoutcount++;
      *(timer_adress) &= ~0x1;  
      
    
      if (timeoutcount >= 10){
        timeoutcount = 0;

        if(mytime == (0x5959)){
        hours++;
        }
        if(hours == 10){
          hours = 0;
          tenth_hour++;
        }
        if(tenth_hour == 2 && hours == 4){
          tenth_hour = 0;
          hours = 0;
        } 

        tick( &mytime );     // Ticks the clock once
          
        time2string( textstring, mytime ); // Converts mytime to string
        //display_string( textstring ); //Print out the string 'textstring'
        display_time(mytime); //display time
        
      }
  }
    
    }

/* Add your code here for initializing interrupts. */
void labinit(void){
  volatile int* button_adress1 = (volatile int*) 0x040000e0;
    //volatile int* button_adress2 = (volatile unsigned short*);
  
    *(button_adress1 +1) = 0x0; //Sets direction to input 
    
    //Test output: vad finns på addressen
    print_dec(*button_adress1);

    volatile unsigned short* timer_adress = (volatile unsigned short*) 0x04000020;

  // Ställ in timeout-perioden till 3 000 000(-1,nollindexerat) cykler (100 ms vid 30 MHz)
  *(timer_adress + 4) = 2999999 & 0xFFFF;  // Lägsta 16 bitarna
  *(timer_adress + 6) = (2999999 >> 16) & 0xFFFF;  // Högsta 16 bitarna

  // Starta timern
  *(timer_adress + 2) = 0x7;  // START och CONT och ITO satt till 1


asm volatile ("csrsi mie,16");
asm volatile ("csrsi mstatus,3");
}

/* Your code goes into main as well as any needed functions. */
int main() {

  // Call labinit()
  labinit();

  while(1){

  }

}


