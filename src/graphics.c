#include <include/graphics.h>


int main(){
    volatile char *VGA = (volatile char*) 0x08000000; // Create a pointer to the VGA pixel buffer. This is the "drawing" area.
    for (int i = 0; i < 320*480; i++)
        VGA[i] = i / 320; // Fill the drawing area with some values.
    unsigned int y_ofs= 0;
    volatile int *VGA_CTRL = (volatile int*) 0x04000100; // Create a pointer to the VGA DMA
    while (1){
        *(VGA_CTRL+1) = (unsigned int) (VGA+y_ofs*320); // Update the backbuffer to point to the VGA pixel buffer + 320*y_ofs
        *(VGA_CTRL+0) = 0; // Write to the backbuffer control register to perform the swap.
        y_ofs= (y_ofs+ 1) % 240; // Increase y_ofs by one and wrap around when reaching 240.
        for (int i = 0; i < 1000000; i++)
            asm volatile ("nop"); // Delay for some unit of time
    }
}