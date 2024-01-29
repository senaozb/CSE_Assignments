/* This program counts the day between the given dates
   Level: Medium                                       */

#include <stdio.h>

int leap_year_check(int year) 
{
   /* leap year is divisible by 400 */
   if (year % 400 == 0) 
		return 1;
   /* if it is divisible by 100 but it is not divisible by 400, it is not leap year */
   else if (year % 100 == 0)
		return 0;
   /* if it is not divisible by 100 but it is divisible by 4, it is leap year */
   else if (year % 4 == 0) 
		return 1;
   else 
		return 0;
}

int count_day_between_dates(int start_day, int start_month, int start_year, int end_day, int end_month, int end_year)
{
    int day = 0;
    int total_day = 0;
    int leap_result = 0;
    int i, j, k;
    int month_day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    /* input check */
	int month_ch[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
	if(start_year < 0 || start_month > 12 || start_day < 0 || start_day > month_ch[start_month-1]) 
	{
		printf("The input is invalid\n");
		return -1;
	}
	if(end_year < 0 || end_month > 12 || end_day < 0 || end_day > month_ch[end_month-1]) 
	{
		printf("The input is invalid\n");
		return -1;
	}
	
    if(start_month > end_month) 
	{	
		for(k = start_year + 1; k < end_year; k++) /*there is no year between start and end months so it is accurate not to add 1 year*/
		{
			leap_result = leap_year_check(k);
			if(leap_result == 0)
				total_day += 365;
			else
				total_day += 366;
		}
		for(i = start_month; i <= 12; i++) /*to calculate the left days in start year*/
		{
			leap_result = leap_year_check(start_year);
			if(i == 2 && leap_result == 1)
				total_day += month_day[i-1] + 1;
			else
				total_day += month_day[i-1];
		}
		for(j = 1; j <= end_month; j++) /*to calculate the left days in end year*/
		{
			leap_result = leap_year_check(end_year);
			if(j == 2 && leap_result == 1)
				total_day += month_day[j-1] + 1;
			else
				total_day += month_day[j-1];
		}
		
		total_day -= start_day; /*to delete the extra days in start year*/
	
		leap_result = leap_year_check(end_year);
		if(end_month == 2 && leap_result == 1) /*to delete the extra days in end year*/
			total_day -= (month_day[end_month-1] + 1 - end_day);
		else
			total_day -= (month_day[end_month-1] - end_day);	
	}
	else if(start_month < end_month) 
	{
		for(k = start_year; k < end_year; k++) /*there is a year between start and end months so it is accurate to add 1 year*/
		{
			leap_result = leap_year_check(k);
			if(leap_result == 0)
				total_day += 365;
			else
				total_day += 366;
		}
		for(j = start_month; j <= end_month; j++) /*to calculate the left days in end year*/
		{
			leap_result = leap_year_check(end_year);
			if(j == 2 && leap_result == 1)
				total_day += month_day[j-1] + 1;
			else
				total_day += month_day[j-1];
		}
		
		total_day -= start_day; /*to delete the extra days in start year*/
	
		leap_result = leap_year_check(end_year);
		if(end_month == 2 && leap_result == 1) /*to delete the extra days in end year*/
			total_day -= (month_day[end_month-1] + 1 - end_day);
		else
			total_day -= (month_day[end_month-1] - end_day);
	}
	else
	{
		for(k = start_year; k < end_year; k++) /*there is almost a year between start and end months so it is accurate to add 1 year*/
		{
			leap_result = leap_year_check(k);
			if(leap_result == 0)
				total_day += 365;
			else
				total_day += 366;
		}
		if(start_day > end_day) /*to delete the overlapped days*/
		{
			day = start_day - end_day;
			total_day -= day;
		}
		else /*to add the left days*/
		{
			day = end_day - start_day;
			total_day += day;
		}
	}
	return total_day;
}

void test_count_day_between_dates() 
{
	int start_day = 14, start_month=2, start_year=2022;
	int end_day = 21, end_month=2, end_year=2022;
	int numdays;
	numdays = count_day_between_dates(start_day, start_month, start_year, end_day, end_month, end_year);

	if(numdays != -1) /*for error message*/
	{
		printf("The number of days between %02d-%02d-%04d and ",start_day, start_month, start_year);
		printf("%02d-%02d-%04d is %d.\n",end_day, end_month, end_year, numdays);
	}
}

int main()
{
	test_count_day_between_dates();
	return 0;
}