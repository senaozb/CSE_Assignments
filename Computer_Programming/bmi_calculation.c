/* This program calculates the bmi value
   Level: Easy                          */

#include <stdio.h>

void bmi_calculation()
{
	/*BMI = weight(kg)/height(m)^2*/
	double w,h,bmi;
	printf("Please enter weight(kg): ");
	scanf("%lf", &w);
	printf("Please enter height(m): ");
	scanf("%lf", &h);
	
	bmi = w/(h*h);
	
	/*to print the results*/
	if (bmi < 16.0)
		printf("Your category: Severely Underweight.\n");
	else if(bmi >= 16.0 && bmi < 18.5)
		printf("Your category: Underweight.\n");
	else if(bmi >= 18.5 && bmi < 25.0)
		printf("Your category: Normal.\n");
	else if(bmi >= 25.0 && bmi < 30.0)
		printf("Your category: Overweight.\n");
	else if(bmi >= 30.0)
		printf("Your category: Obese.\n");
}

int main()
{
	bmi_calculation();
	return 0;
}
