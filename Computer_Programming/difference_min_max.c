/* This program calculates the difference between the maximum and minimum numbers
   Level: Easy                                                                    */

#include <stdio.h>

void difference_max_min()
{
	double max , min, diff;
	double num1, num2, num3, num4, num5;
	printf("Please enter 5 numbers: ");
	scanf("%lf %lf %lf %lf %lf", &num1, &num2, &num3, &num4, &num5);

	/*find the maximum number*/
	max = num1;
	if(num2 > max)
		max = num2;
	if(num3 > max)
		max = num3;
	if(num4 > max)
		max = num4;
	if(num5 > max)
		max = num5;
		
	/*find the minimum number*/	
	min = num1;
	if(num2 < min)
		min = num2;
	if(num3 < min)
		min = num3;
	if(num4 < min)
		min = num4;
	if(num5 < min)
		min = num5;	
	
	printf("Maximum number is %g\n", max);
	printf("Minimum number is %g\n", min);
	
	diff = max - min;
	printf("Difference between maximum and minimum is %g\n", diff);
}

int main()
{
	difference_max_min();
	return 0;
}
