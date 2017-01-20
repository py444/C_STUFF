#include "header.h"

WINDOW *calc_win;

void createCalculWin()
{
	calc_win = newwin(20, 100, 10, 50);
	
    box(calc_win, 0 , 0);
    wrefresh(calc_win);
}