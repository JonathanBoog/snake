#ifdef GAME_LOGIC_H
#define GAME_LOGIC_H


int check_collision(void);
int get_correct_board_color(int row, int col);
void spawn_food(void);
void check_food_collision(void);
void update_snake(void);
void game_over(void);

#endif


