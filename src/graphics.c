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


    /*
    volatile char *VGA = (volatile char*) 0x08000000; // Pekare till VGA-pixelbufferten
    unsigned int square_size = 20; // Storleken på varje ruta i pixlar
    unsigned int num_rows = 240 / square_size; // Antal rader
    unsigned int num_cols = 320 / square_size; // Antal kolumner

    // Rita ett schackbrädsliknande mönster med gröna och mörkgröna rutor
    for (unsigned int row = 0; row < num_rows; row++) {
        for (unsigned int col = 0; col < num_cols; col++) {
            // Välj färg baserat på rutans position
            char color = (row + col) % 2 == 0 ? 0x0A : 0x02; // Grön och mörkgrön färg
            for (unsigned int y = 0; y < square_size; y++) {
                for (unsigned int x = 0; x < square_size; x++) {
                    // Beräkna pixelns position i VGA-bufferten
                    unsigned int pixel_index = (row * square_size + y) * 320 + (col * square_size + x);
                    VGA[pixel_index] = color;
                }
            }
        }
    }

    unsigned int y_ofs = 0;
    volatile int *VGA_CTRL = (volatile int*) 0x04000100; // Pekare till VGA DMA

    while (1) {
        *(VGA_CTRL + 1) = (unsigned int)(VGA + y_ofs * 320); // Uppdatera backbuffer för att peka på VGA-pixelbufferten
        *(VGA_CTRL + 0) = 0; // Utför en swap
        y_ofs = (y_ofs + 1) % 240; // Öka y_ofs och wrappa runt vid 240
        for (int i = 0; i < 1000000; i++)
            asm volatile("nop"); // Fördröjning
    }

    */
}