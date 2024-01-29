/* This program finds the root of 3rd and 4th order equations
   Level: Medium                                             */

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

double root3(double a0, double a1, double a2, double a3, double xs, double xe)
{
	/* This function is based on Bisection Method */
	double e=0.0000001; /* error rate */
	double c;
	c = xs;
 
    while ((xe-xs) >= e)
    {
    	c = (xs+xe)/2;
    	/* The function is a0*x^3 + a1*x^2 + a2*x + a3 */
    	double func_result_c = func_calc_3(a0, a1, a2, a3, c);
    	double func_result_xs = func_calc_3(a0, a1, a2, a3, xs);
    	
        if (func_result_c == 0)
            break;
        else if (func_result_c*func_result_xs < 0)
            xe = c;
        else
        	xs = c;
    }
    
    return c;
}


double root4(double a0, double a1, double a2, double a3, double a4, double xs, double xe)
{
    /* This function is based on Bisection Method */
	double e=0.0000001; /* error rate */
	double c;
	c = xs;
 
    while ((xe-xs) >= e)
    {
    	c = (xs+xe)/2;
    	/* The function is a0*x^4 + a1*x^3 + a2*x^2 + a3*x + a4 */
    	double func_result_c = func_calc_4(a0, a1, a2, a3, a4, c);
    	double func_result_xs = func_calc_4(a0, a1, a2, a3, a4, xs);
    	
        if (func_result_c == 0)
            break;
        else if (func_result_c*func_result_xs < 0)
            xe = c;
        else
        	xs = c;
    }
    
    return c;
}

/*Test functions*/
void test_root_finder3() 
{
	double root_known = 1.0;
	double root_found;
	root_found = root3(1.2, 1.0, -1.0, -1.2, root_known-0.1, root_known+0.1);
	printf("Ground truth root: %f vs found root: %f\n", root_known, root_found);
}

void test_root_finder4() 
{
	double root_known = 2.0;
	double root_found;
	root_found = root4(2.0, -20.0, 70.0, -100.0, 48.0, root_known-0.1, root_known+0.1);
	printf("Ground truth root: %f vs found root: %f\n", root_known, root_found);
}

int main(void)
{
	test_root_finder3();
    test_root_finder4();
	return (0);
} 
