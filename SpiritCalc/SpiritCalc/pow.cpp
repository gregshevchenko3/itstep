#include "pow.h"
#include <cmath>
#include <cfenv>
#include <stdexcept>


double calc_pow_impl::operator()(double & base, double & exp) const
{
	double result = 0.0;
	if (base < 0) {
		double whole, fraction;
		// exp == whole + fraction, �� whole - ���� ������� ��������� ������� exp, � fraction - ������ ������� ���������� ������� exp
		fraction = std::modf(exp, &whole);
		 
		if (fraction == 0) {
			if (whole == 0) return 1.0; // 0-� ������ ������� ����� ������ == 1.
			else result = std::pow(base, exp); // 
		} else 
		// � ������ ������� - �������� ���� ������ �������. ������� �������� ������� ������, �� ������� ����� ����� � ���������� 1/fraction ����� base.
		// ���� base < 0, � 1/fraction - ����� �����, �� ������� ������ � ������ ��������� �����, ������ ������� ��������� �����, �� -std::pow(base, exp);
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