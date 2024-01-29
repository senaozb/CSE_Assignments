/* This program prints the polynomials with given degree and coefficients
   Level: Medium                                                          */

#include <stdio.h>

void write_polynomials()
{
	int check_order = 0, i = 0, a0;
	float number, mod;
	char c;
	
	printf("Enter your polynomial as an order of [n a_n a_n-1 a_n-2 ... a_0] ");
    printf("(n is degree and a_n is coefficent e.g 5x^4+6x^3+7x^2 is 4 5 6 7 0 0):");
    
	while(c != '\n')
	{
		scanf("%f%c", &number, &c);
		/*get the value of max power*/
		if(check_order == 0)
		{
			a0 = number;
			check_order++;
			printf("p(x) = ");
		}
		else
		{
			if(number == 0) /*not to print 0x*/
			{
				i++;
				continue;
			}
	
			if(number > 0 && i != 0) /*to print '+' sign for positive numbers*/
				printf("+");
					
			if(number == 1) /*not to print 1x*/
			{
				if(a0-i == 1) /*not to print x^1*/
					printf("x");
				else if(a0-i == 0) /*not to print x^0*/
					printf("1");	
				else
					printf("x^%d", a0-i);	
			}
			else if(number == -1) /*not to print -1x*/
			{
				if(a0-i == 1) /*not to print x^1*/
					printf("-x");
				else if(a0-i == 0) /*not to print x^0*/
					printf("-1");	
				else
					printf("-x^%d", a0-i);	
			}
			else
			{
				mod = number - (int)(number/1.0)*1.0; /* modulus for float numbers = x - (int)(x/y)*y */
				if(mod == 0)
				{
					if(a0-i == 1) /*not to print x^1*/
						printf("%.fx", number);
					else if(a0-i == 0) /*not to print x^0*/
						printf("%.f", number);	
					else
						printf("%.fx^%d", number, a0-i);	
				}
				else
				{
					if(a0-i == 1) /*not to print x^1*/
						printf("%.1fx", number);
					else if(a0-i == 0) /*not to print x^0*/
						printf("%.1f", number);	
					else
						printf("%.1fx^%d", number, a0-i);	
				}
				
			}
			i++; /*continue with next order*/
		}	
	}
	printf("\n");
}

int main()
{
	write_polynomials();
	return 0;
}