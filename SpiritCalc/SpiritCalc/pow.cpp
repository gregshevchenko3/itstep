#include "pow.h"
#include <cmath>
#include <cfenv>
#include <stdexcept>


double calc_pow_impl::operator()(double & base, double & exp) const
{
	double result = 0.0;
	if (base < 0) {
		double whole, fraction;
		// exp == whole + fraction, де whole - ціла частина показника степеню exp, а fraction - дробна частина показнника степеня exp
		fraction = std::modf(exp, &whole);
		 
		if (fraction == 0) {
			if (whole == 0) return 1.0; // 0-й степінь дійсного числа завжди == 1.
			else result = std::pow(base, exp); // 
		} else 
		// В даному випадку - цікавить лише дробна частина. Дробний показник степеня означає, що потрібно взяти корінь з показником 1/fraction числа base.
		// Якщо base < 0, а 1/fraction - парне число, то дійсного кореня з парним показникм неіснує, інакше потрібно визначати корінь, як -std::pow(base, exp);
		if (std::modf(1 / (2 * fraction), &fraction) > 0) {
			result = -std::pow(-base, exp);
		}
		else
			throw std::invalid_argument("Unable to calculate the square root of a negative number");
	}
	else 
		result = std::pow(base, exp);
	return result;
}