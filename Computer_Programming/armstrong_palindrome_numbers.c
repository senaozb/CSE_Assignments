/* This program finds whether the given number is an armstrong number or palindrome number
   Level: Easy                                                                            */

#include <stdio.h>

void find_armstrong_palindrome_numbers()
{
	int number, given_number, reverse_number = 0, remain_of_number;
	int sum = 0;
	int check_palindrome = 0, check_armstrong = 0;
	float mod, n;
    printf("Please enter an integer number: ");
    do
	{
		scanf("%f", &n);
		mod = n - (int)(n/1.0)*1.0;
		
		if(n < 0.0) /*warn the user to enter non-negative number*/
			printf("Please enter a non-negative integer number: ");
		else if(mod != 0.0) /*warn the user to enter an integer number*/
			printf("Please enter an 'integer' number: ");
				
	}
	while(n < 0 || mod != 0.0);
	
	number = (int)n;
    given_number = number; /*store the given number to compare*/

    /*reverse the given number*/
    while (number != 0) 
	{
        remain_of_number = number % 10; /*get the digit*/
        sum += remain_of_number*remain_of_number*remain_of_number; /*calculate digit^3 for armstrong number*/
        reverse_number = reverse_number * 10 + remain_of_number; /*store the reversed version for palindrome number*/
        number /= 10; /*update to get other digits*/
    }

    /*check if it is palindrome*/
    if (given_number == reverse_number)
        check_palindrome = 1;
    /*check if it is armstrong*/
    if(sum == given_number)
    	check_armstrong = 1;
    /*print the result*/	
	if(check_palindrome == 1 && check_armstrong == 1)
		printf("This number is both Palindrome and Armstrong number.\n");
	else if(check_palindrome == 1)	
		printf("This number is only Palindrome number.\n");
	else if(check_armstrong == 1)	
		printf("This number is only Armstrong number.\n");
	else
		printf("This number does not satisfy any special cases.\n");
}

int main()
{
	find_armstrong_palindrome_numbers();
	return 0;
}
