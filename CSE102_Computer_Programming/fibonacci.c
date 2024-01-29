/* This program prints the fibonacci sequence
   Level: Easy                               */

#include <stdio.h>

void calculate_fibonacci_sequence()
{
	while(1)
	{
		int digit, first, second, next, i;
		char ch;
		
		printf("Please enter term(s) number (type * to exit): ");
		do /*check the input*/
		{
 			scanf("%d", &digit); 
			ch = getchar();
		
			if(ch == '*') /*check to stop program*/
				break;
			if(ch != '\n') /*warn the user for the input*/
				printf("Please enter numeric term(s) number: ");
			if(digit <= 0) /*warn the user for the input*/
			{
				if(ch == '\n')
					printf("Please enter positive term(s) number: ");	
			}	
		}
		while(digit <= 0 || ch != '\n');
		if(ch == '*') /*stop the program*/
			break;
		
		printf("Fibonacci Sequence: \n"); 
		first = 0;    
		second = 1;    
		printf("%d\n", first);      
  
  		/*print and calculate fibonacci sequence*/
		for (i = 1; i < digit; i++)    
  		{        
   	 		next = first + second;        
   	 		first = second;        
   	 		second = next;        
   	 		printf("%d\n", first);   
 	 	}       
	}
}

int main()
{
	calculate_fibonacci_sequence();
	return 0;
}
