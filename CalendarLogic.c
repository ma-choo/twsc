int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

char *monthName[] =
{
	"January", "February", "March", "April",
	"May", "June", "July", "August",
	"September", "October", "November", "December"
};

char *dayName[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

typedef struct
{
	int day;
	int month;
	int year;
	char *name[32];
} event;

void determineLeapYear(int year)
{
	if((year % 4 == 0 && year % 100 != 0) || year % 400 ==0)
		daysInMonth[1] = 29;
	else
		daysInMonth[1] = 28;
}

int getFirstDayOfMonthOffset(int Month, int Year)
{
	Month++; // Normalize month for the Zeller algorithm
    int Day = 1, zMonth, zYear, Zeller;

    if(Month < 3)
        zMonth = Month+10;
    else
        zMonth = Month-2;

    if(zMonth > 10)
        zYear = Year-1;
    else
        zYear = Year;

    Zeller = ((int)((13 * zMonth - 1) / 5) + Day + zYear % 100 +
             (int)((zYear % 100) / 4) - 2 * (int)(zYear / 100) +
             (int)(zYear / 400) + 77) % 7;

    return Zeller;
}

