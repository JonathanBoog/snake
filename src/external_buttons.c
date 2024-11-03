#include <include/external_buttons.h>

// C function showing how to do time delay
#include <stdio.h>
// To use time library of C
#include <time.h>

void delay(int number_of_seconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + number_of_seconds);
}


int get_btn(void){
  volatile int* button_adress = (volatile int*) 0x40000d0;
  return *button_adress & 0x1;
}

init(void){
    volatile int* button_adress1 = (volatile int*) 0x040000e0;
    //volatile int* button_adress2 = (volatile unsigned short*);

    *(button_adress1 +1) = 0x0; //Sets direction to input 
    //Kanske använda edgecase också? 
}

// Driver code to test above function
int main(){
    init();
    while(1){
        if(get_btn())
            printf("Button is pressed");
        // delay of one second
        delay(1000);

        
    }


    return 0;
}