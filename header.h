#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include <ncurses.h>

#define NAV_BTN_COUNT 3

typedef struct
{
	int width;
	int height;
}Size;

typedef struct
{
	int x;
	int y;
}COORD;

typedef struct
{
	COORD scr_pos;
	Size sizeBtn;
	char* text;
	bool isSelected;
	bool isHovered;
}Button;

Button CreateBtn(COORD scr_pos, Size sizeBtn, char* text, bool isSelected, bool isHovered);

bool isHovered(Button btn, int x, int y);

Button HoveredBtn(Button btnArray[], int y, int x);

Button ClickedBtn(Button btnArray[], int y, int x);

void DrawNavBtn(Button btnArray[]);

void DrawBtn(Button btn);

void Goto(COORD position);

void initmouse(void);