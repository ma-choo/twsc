#include <time.h>
#include <curses.h>
#include "CalendarLogic.c"

int main()
{
	time_t s, val = 1;
	struct tm* current_time;
	s = time(NULL);
	current_time = localtime(&s);

	int currentDay, currentMonth, currentYear;
	int workingDay, workingMonth, workingYear;
	int nextMonth, lastMonth;
	int day_y, day_x, key;

	currentDay = workingDay = current_time->tm_mday;
	currentMonth = workingMonth = current_time->tm_mon;
	currentYear = workingYear = current_time->tm_year + 1900;

	determineLeapYear(currentYear);

	initscr();
	noecho();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	
	int scr_w, scr_h, events_h, events_w; // Window dimensions will be initialized dynamically to account for the width and height of the terminal
	int	cal_h = 9; // as well as the space occupied by the Calendar window, which is constant
	int cal_w = 22;

	getmaxyx(stdscr, scr_h, scr_w); // Get the width and height of the terminal

	// Make the height and width of the events panel divisible by 6 and 7, respectively. Account 1px for the window border
	// hfactor and wfactor will be used to separate the days
	int hfactor = (scr_h - 1) / 6;
	int wfactor = ((scr_w - cal_w) - 1) / 7;
	events_h = (hfactor * 6) + 1;
	events_w = (wfactor * 7) + 1;

	// DRAW EVENTS WINDOW ------------------------------------------------------
	WINDOW *Events = newwin(events_h, events_w, 0, cal_w);

	day_y = 1;
	day_x = getFirstDayOfMonthOffset(currentMonth, currentYear);

	for(int d = 1; d <= daysInMonth[currentMonth]; d++)
	{
		if(d == currentDay)
			wattron(Events, COLOR_PAIR(1));
			// wattron(Events, A_REVERSE);
		
		mvwprintw(Events, day_y, day_x * wfactor, "%*d", wfactor, d);
		
		if(++day_x > 6)
		{
			day_y += hfactor;
			day_x=0;
		}
	
		wattroff(Events, COLOR_PAIR(1));
		// wattroff(Events, A_REVERSE);
	}

	// Draw window border and day separators
	for(int y = 1; y < 6; y++)
	{	
		mvwhline(Events, hfactor * y, 0, ACS_HLINE, events_w);
	}
	for(int x = 1; x < 7; x++)
	{
		mvwvline(Events, 0, wfactor * x, ACS_VLINE, events_h);
	}
	box(Events, 0, 0);

	for (int x = 0; x < 7; x++)
	{
		mvwprintw(Events, 0, x * wfactor + 1, "%s", dayName[x]);
	}

	// DRAW CALENDAR WINDOW ----------------------------------------------------------------
	WINDOW *Calendar = newwin(cal_h, cal_w, 0, 0);
	box(Calendar, 0, 0);

	// Print calendar header
	mvwprintw(Calendar, 0, 1, "%s %d", monthName[currentMonth], currentYear);
	mvwprintw(Calendar, 1, 2, "S  M  T  W  T  F  S");

	day_y = 2;
	day_x = getFirstDayOfMonthOffset(currentMonth, currentYear);

	for(int d = 1; d <= daysInMonth[currentMonth]; d++)
	{
		if(d == currentDay)
			wattron(Calendar, COLOR_PAIR(1));
			// wattron(Calendar, A_REVERSE);
		
		mvwprintw(Calendar, day_y, day_x * 3 + 1, "%2d", d);
		if(++day_x > 6)
		{
			day_y++;
			day_x=0;
		}
	
		wattroff(Calendar, COLOR_PAIR(1));
		// wattroff(Calendar, A_REVERSE);
	}

	refresh();
	wrefresh(Calendar);
	wrefresh(Events);

	key = getch();

	delwin(Events);
	delwin(Calendar);
	endwin();

	return 0;
}
