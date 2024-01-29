/* This program finds whether the given number is perfect number or harmonic number
   Level: Easy                                                                     */

#include <stdio.h>

void decide_perfect_harmonic_number()
{
	while(1)
	{
		int number, sum = 0, divisor = 0, i; 
		double divisor_sum = 0, mod, harmonic;
		char ch;
		printf("Please enter your number (type * to exit): ");
		do /*check the input*/
		{
 			scanf("%d", &number); 
			ch = getchar();
		
			if(ch == '*') /*check to stop program*/
				break;
			if(ch != '\n') /*warn the user for the input*/
				printf("Please enter a numeric number: ");
			if(number < 0) /*warn the user for the input*/
			{
				if(ch == '\n')
					printf("Please enter a positive number: ");	
			}	
		}
		while(number < 0 || ch != '\n');
		if(ch == '*')
			break;
			
		printf("Natural Number Divisors: ");
		for(i = 1 ; i < number ; i++)   
    	{   
    		/*find the divisors*/
        	if(number % i == 0)   
        	{
        		sum += i;
        		divisor += 1;
        		divisor_sum += (double)1/i; /*calculate the harmonic mean*/
        		printf("%d, ", i);
			}
    	}    
    	printf("%d\n", number); /*print the number itself for natural divisor list*/
    
    	/*print if it is a perfect number*/
		if(number == 0)
			printf("Is Perfect Number? : No\n");
		else if(sum == number)
			printf("Is Perfect Number? : Yes\n");	
		else
			printf("Is Perfect Number? : No\n");
	
		/*print if it is a harmonic number*/
		divisor_sum += (double)1/number;
		harmonic = (divisor + 1)/divisor_sum;
		
		mod = harmonic - (int)(harmonic/1.0)*1.0;
		
		if(number == 0)
			printf("Is Harmonic Divisor Number? : No\n");
		else if(mod == 0.0)
			printf("Is Harmonic Divisor Number? : Yes\n");
		else
			printf("Is Harmonic Divisor Number? : No\n");
	}
}

int main()
{
	decide_perfect_harmonic_number();	
	return 0;
}
