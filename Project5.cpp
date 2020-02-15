#include <iostream>

int factorial(int n) {
	return (n == 1)? n : (n * factorial(n - 1));
}
/**a^b*/
double power_pos(int a, int b) {
	return (b == 1)? a : (a * power_pos(a, b - 1));
}
double power(int a, int b) {
	return (b > 0) ? power_pos(a, b) : 
		(b < 0) ? 1/power_pos(a, -b) : 1.0;
}


using uint = unsigned int;
void fib_to(uint a, uint p1, uint p2) {
	std::cout << p1 + p2 << " ";
	if ((p1 + 2 * p2) <= a) fib_to(a, p2, p1 + p2);
}
void fib_to(uint a) {
	std::cout << "1 2 ";
	fib_to(a, 1, 2);
	std::cout << std::endl;
}


void fib_count(uint a, uint p1, uint p2) {
	std::cout << p1+p2<< " ";
	if (a > 1)fib_count(a - 1, p2, p1 + p2);
}
void fib(uint count) {
	std::cout << "1 2 ";
	fib_count(count-2, 1, 2);
	std::cout << std::endl;
}

int main()
{
	std::cout << factorial(5) << std::endl;

	std::cout << power(2, 2) << std::endl;
	std::cout << power(2, -2) << std::endl;
	std::cout << power(2, 0) << std::endl;

	fib_to(55);
	fib(20);
}