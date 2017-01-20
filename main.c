/* COMPILE WITH XTERM : TERM=xterm-1003
 * CONFIGURE MOUSE ARROW COMMAND : xterm -xrm "XTERM*pointerShape: left_ptr"	
 * full-command : xterm -geometry 400x400 -xrm "XTERM*pointerShape: left_ptr"	
 */

#include "header.h"
#include "calculatrice.c"

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

Button btnNavArray[10];

WINDOW *calc_win;

int main(void)
{
	initscr();
	initmouse();
	curs_set(0); // HIDE CURSOR
	noecho();
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

	COORD btn_position = { .x = 10, .y = 10};
	Size btn_size = { .width = 30, .height = 2};
	Button btn_1 = CreateBtn(btn_position, btn_size, "Calculatrice", true, false);
	
	btn_position.y += 3;
	Button btn_2 = CreateBtn(btn_position, btn_size, "Equation 1er Deg", false, false);

	btn_position.y += 3;
	Button btn_3 = CreateBtn(btn_position, btn_size, "HELLO4", false, false);

	btnNavArray[0] = btn_1; btnNavArray[1] = btn_2; btnNavArray[2] = btn_3;
	DrawNavBtn(btnNavArray);

	refresh();

	createCalculWin();
	
	int c;
	char* btnName;
	MEVENT event;
	while(1) {
		c = wgetch(stdscr);
		if (KEY_MOUSE == c) {
            if (OK == getmouse(&event)) {
            	move(0,0);
                printw("y : %d - x : %d", event.y, event.x);

                HoveredBtn(btnNavArray, event.y, event.x);

                if(event.bstate & BUTTON1_PRESSED || event.bstate & BUTTON1_CLICKED)
				{
					ClickedBtn(btnNavArray, event.y, event.x);
				}

				DrawNavBtn(btnNavArray);
                refresh();
            }
        }
	}
	
	getch();
	endwin();

	return 0;
}

Button ClickedBtn(Button btnArray[], int y, int x)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		Button btn = btnArray[i];
		if (x > btn.scr_pos.x && x < btn.scr_pos.x + btn.sizeBtn.width 
		&& y > btn.scr_pos.y && y < btn.scr_pos.y + btn.sizeBtn.height)
		{
			btnArray[i].isSelected = true;
			int j;
			for (j = 0; j < 3; j++)
			{
				if (btnArray[j].text != btnArray[i].text) btnArray[j].isSelected = false;
			}
			return btn;
		}
	}
	
	Button btn;
	btn.text = "A";
	return btn;	
}

Button HoveredBtn(Button btnArray[], int y, int x)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		Button btn = btnArray[i];
		if (x > btn.scr_pos.x && x < btn.scr_pos.x + btn.sizeBtn.width 
		&& y > btn.scr_pos.y && y < btn.scr_pos.y + btn.sizeBtn.height)
		{
			btnArray[i].isHovered = true;
			return btn;
		}else {
			btnArray[i].isHovered = false;
		}
	}
	
	Button btn;
	btn.text = "A";
	return btn;
}

void DrawNavBtn(Button btnArray[])
{
	int i;
	for(i = 0; i < NAV_BTN_COUNT; i++)
	{
		DrawBtn(btnArray[i]);
	}
}

void DrawBtn(Button btn)
{
	Size sizeBtn = btn.sizeBtn;
	COORD scr_pos = btn.scr_pos;
	char* text = btn.text;
	bool isSelected  = btn.isSelected;
	bool isHovered = btn.isHovered;

	COORD T_RIGHT = { .x = scr_pos.x + sizeBtn.width, .y = scr_pos.y}; // TOP RIGHT CORNEER POSITION
	COORD B_LEFT = { .x = scr_pos.x, .y = scr_pos.y + sizeBtn.height}; // BOTTOM LEFT CORNEER POSITION
	COORD B_RIGHT = { .x = T_RIGHT.x, .y = B_LEFT.y}; // BOTTOM RIGHT CORNEER POSITION

	attrset(A_NORMAL); // Reset console to normal text

	// Draw top border
	Goto(scr_pos);
	hline(ACS_HLINE, sizeBtn.width);
	
	// Draw left border
	vline(ACS_VLINE, sizeBtn.height);
	
	// Draw Bottom Border
	Goto(B_LEFT);
	hline(ACS_HLINE, sizeBtn.width);
	
	// Draw right border
	Goto(T_RIGHT);
	vline(ACS_VLINE, sizeBtn.height);

	// Draw BTN corners
	Goto(scr_pos); addch(ACS_ULCORNER);
	Goto(T_RIGHT); addch(ACS_URCORNER);
	Goto(B_LEFT); addch(ACS_LLCORNER);
	Goto(B_RIGHT); addch(ACS_LRCORNER);

	// Draw Text
	COORD T_Pos = { .x = scr_pos.x + 1, .y = scr_pos.y + (sizeBtn.height/2)};
	
	if (isSelected)
	{
		Goto(T_Pos);
		attron(A_STANDOUT);
		hline(' ', sizeBtn.width - 1);
	}

	if (isHovered)
	{
		Goto(T_Pos);
		attron(A_STANDOUT);
		hline(' ', sizeBtn.width - 1);
	} else {
		Goto(T_Pos);
		attron(A_NORMAL);
		hline(' ', sizeBtn.width - 1);
	}
	
	T_Pos.x += floor((sizeBtn.width / 2) - floor(strlen(text) / 2));

	Goto(T_Pos);
	printw("%s", text);
	attrset(A_NORMAL);
}

Button CreateBtn(COORD scr_pos, Size sizeBtn, char* text, bool isSelected, bool isHovered)
{
	/* Return Button Type */
	Button btn = { .scr_pos = scr_pos, .sizeBtn = sizeBtn, .text = text, .isSelected = isSelected, 
					.isHovered = isHovered};
	return btn;
}

bool isHovered(Button btn, int x, int y)
{
	if (x > btn.scr_pos.x && x < btn.scr_pos.x + btn.sizeBtn.width 
		&& y > btn.scr_pos.y && y < btn.scr_pos.y + btn.sizeBtn.height) return true;

	return false;
}

void Goto(COORD position)
{
	move(position.y, position.x);
}

void initmouse()
{
	keypad(stdscr, TRUE); /* Enable keypad mode */
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL); /* Report all mouse events */
}