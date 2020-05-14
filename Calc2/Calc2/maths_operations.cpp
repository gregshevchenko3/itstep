#include "maths_operations.h"

double pow_operation(double base, double exp) {
	double result = 0.0;
	if (base < 0) {
		double whole, fraction;
		fraction = std::modf(exp, &whole);

		if (fraction == 0) {
			if (whole == 0) return 1.0; // 0-й степінь дійсного числа завжди == 1.
			else result = std::pow(base, exp); // 
		}
		else
			if (std::modf(1 / (2 * fraction), &fraction) > 0) {
				result = -std::pow(-base, exp);
			}
			else
				MathError("Unable to calculate the square root of a negative number", BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
	}
	else
		result = std::pow(base, exp);
	return result;
}

double factorial_operation(double n)
{
	double integral_n = 1.0;
	if(modf(n, &integral_n) > 0 || n < 0) MathError("Unable to calculate factorial of a fraction or negative number", BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
	if (integral_n == 0.0) 
		return 1.0;
	else
		integral_n *= factorial_operation(integral_n - 1);
	return integral_n;
}
