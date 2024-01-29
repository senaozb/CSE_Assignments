/* This program finds the result of 3rd and 4th order definite integrals
   Level: Medium                                                        */

#include <stdio.h>

double func_calc_3(double a0, double a1, double a2, double a3, double x)
{
	double result = a0*x*x*x + a1*x*x + a2*x + a3;
	return result;
}

double func_calc_4(double a0, double a1, double a2, double a3, double a4, double x)
{
	double result = a0*x*x*x*x + a1*x*x*x + a2*x*x + a3*x + a4;
	return result;
}

double integral3(double a0, double a1, double a2, double a3, double xs, double xe, double delta)
{ 
	/* This function is based on Trapezoidal Rule */
	/* The function is a0*x^3 + a1*x^2 + a2*x + a3 */
	/* Formula: T = (delta/2)*[f(x0)+2f(x1)+2f(x2)+f(x3)] for from x0 to x3 with 3 subintervals */
    /* Delta is step size */
	
	double func_result_xe = func_calc_3(a0, a1, a2, a3, xe); /* to calculate the results of the upper and lower boundries */
    double func_result_xs = func_calc_3(a0, a1, a2, a3, xs);
    
    double result = func_result_xe + func_result_xs; 
  	
	int i, n = (xe - xs)/delta; /* n for step number */
    for (i = 1; i < n; i++) /* this loop is used to calculate inner results */
	{
		double x_i = xs + i*delta;
		double func_result = func_calc_3(a0, a1, a2, a3, x_i);
		result += 2*func_result;
	} 
    
	double net = (delta/2)*result; 
	return net;
}


double integral4(double a0, double a1, double a2, double a3, double a4, double xs, double xe, double delta)
{
    /* This function is based on Trapezoidal Rule */
    /* The function is a0*x^4 + a1*x^3 + a2*x^2 + a3*x + a4 */
	/* Formula: T = (delta/2)*[f(x0)+2f(x1)+2f(x2)+f(x3)] for from x0 to x3 with 3 subintervals */
    /* Delta is step size */

	double func_result_xe = func_calc_4(a0, a1, a2, a3, a4, xe); /* to calculate the results of the upper and lower boundries */
    double func_result_xs = func_calc_4(a0, a1, a2, a3, a4, xs);
    
    double result = func_result_xe + func_result_xs; 
  	
	int i, n = (xe - xs)/delta; /* n for step number */
    for (i = 1; i < n; i++) /* this loop is used to calculate inner results */
	{
		double x_i = xs + i*delta;
		double func_result = func_calc_4(a0, a1, a2, a3, a4, x_i);
		result += 2*func_result;
	} 
    
	double net = (delta/2)*result;
	return net;
}

/*Test functions*/
void test_integral_finder3() 
{
	double result_known = 26.0;
	double result_found;
	result_found = integral3(2.0, 3.0, -5.0, 10.0, 0, 2, 0.0001);
	printf("Expected result: %f vs found result: %f\n", result_known, result_found);
}

void test_integral_finder4() 
{
	double result_known = 2925.0;
	double result_found;
	result_found = integral4(4.0, 2.0, 3.0, -5.0, 10.0, 0, 5, 0.0001);
	printf("Expected result: %f vs found result: %f\n", result_known, result_found);
}

int main(void)
{
	test_integral_finder3();
    test_integral_finder4();
	return (0);
} 