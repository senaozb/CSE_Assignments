/* This program prints the sum of prime numbers between the given numbers
   Level: Easy                                                           */

#include <stdio.h>

int prime_check(int number)
{
	/*check the number whether it is prime or not*/
	/*if it is prime, return 1 --- if not, return 0*/
	int divided = 0, i;
	if(number == 0 || number == 1)
		return 0;
    else
	{
		for (i = 2; i <= number/2; i++)
		{
			if (number % i == 0) 
			{
     	    	divided++;
     	    	return 0;
       		 }
		}
		if(divided == 0)
			return 1;
    }

    return 0;
}
void find_sum_of_prime()
{
	int num1, num2, i, sum = 0, check;
	/*get the inputs from user*/
	do
	{
		printf("Please enter first integer number : ");
		scanf("%d", &num1);
		printf("Please enter second integer number : ");
		scanf("%d", &num2);
		
		if(num1 < 0 || num2 < 0)
			printf("Integers should be positive.\n");
		if(num1 > num2) /*warn the user to enter correctly*/
			printf("First integer should be less than second integer.\n");
	}
	while((num1 > num2)||(num1 < 0)||(num2 < 0));
	/*check all numbers between num1 and num2*/
	for(i = num1 + 1; i < num2; i++)
	{
		check = prime_check(i);
		if(check == 1)
			sum += i;
	}
	printf("Sum of prime numbers between %d and %d : %d \n", num1, num2, sum);
}

int main()
{
	find_sum_of_prime();
	return 0;
}
