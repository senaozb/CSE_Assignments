/* This program finds which day of the week the given date is 
   Level: Easy                                               */

#include <stdio.h>

int find_dayNumber(int day, int month, int year)
{
	/* this function is based on Sakamoto's methods */
	int month_ch[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	int mod[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; /*month length mod 7 -by adding them each other*/
	int result = 0;
	
	/* input check */
	if(year < 0 || month > 12 || day < 0 || day > month_ch[month-1]) 
	{
		printf("The input is invalid\n");
		return -1;
	}
	
    if(month < 3) /*for february*/
        year -= 1;
    
    result = (year + year/4 - year/100 + year/400 + mod[month-1] + day) % 7;
    if(result == 0)
    	result = 7; /*to change the number for sunday*/
    return result;
}

void test_find_dayNumber() 
{
	int date_day = 14, date_month=2, date_year=2022, dayno;
	dayno = find_dayNumber(date_day, date_month, date_year);
	if(dayno != -1) /*for error message*/
		printf("%02d-%02d-%04d is the %dth day of the week!\n",date_day, date_month, date_year, dayno);
}

int main()
{
    test_find_dayNumber();
    return 0;
}