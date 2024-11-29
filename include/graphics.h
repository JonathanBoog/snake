#ifndef GRAPHICS_H
#define GRAPHICS_H

void draw_board(void);
void draw_box(int boxx, int boxy, int color, int leftoffset, int topoffset, int rightoffset,int downoffset);

void set_displays(int display_number, int value);
void display_score(int score, char letter);

#endif