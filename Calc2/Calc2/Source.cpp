#include <iostream>
#include <cstring>
#include <string>


#include "maths_operations.h"
#include "ErrorHandling.h"


/**
 * Пачка парсерів.
 * Кожен парсер приймає вказівник на початок підвиразу, і посилання на змінну, в котрій зберігається результат.
 * Кожен парсер повертає вказівник на перший символ, котрий не є тим, що він здатен розпізнати.
 * Крім того кожен парсер викликає інші парсери операцій, котрі мають пріоритет вищий, ніж у поточного.
 */
char* subexpression(char* subexpression, double& result);
char* unary_minus_(char* expression, double& result);
char* white_spaces_(char* expression, double& result);
char* number_(char* expression, double& result);
char* factorial_(char* expression, double& result);
char* pow_(char* expression, double& result);
char* factors_(char* expression, double& result);
char* addings_(char* expression, double& result);
/* точка входу в пачку парсерів */
double execute(char* expression);

int main() {
	double res = 0.0;
	while (1) {
		std::string line;
		std::cout << "> ";
		std::getline(std::cin, line);

		if (!line.empty()) {
			char* expr = (char*)line.c_str();
			try {
				std::cout << execute(expr) << std::endl;
			}
			catch (const std::exception& e) {
				CatchError(e, expr);
			}
		}
		line.~basic_string();
	}
	return 0;
}
char * subexpression(char * subexpression, double & result)
{
	char* tmp = subexpression;
	subexpression = white_spaces_(subexpression, result);
	if (*subexpression == '(') {
		subexpression = addings_(++subexpression, result);
		if (*subexpression == ')')
			++subexpression;
		else
			if (*subexpression == 0)
				ParserError("')' missing", tmp, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
	}
	else
		subexpression = number_(subexpression, result);
	return white_spaces_(subexpression, result);
}
char * unary_minus_(char * expression, double & result)
{
	if (*expression == '-') {
		expression = factorial_(++expression, result);
		result = -result;
	}
	else 
		expression = factorial_(expression, result);
	return expression;
}
char * white_spaces_(char * expression, double & result)
{
	while (expression && (*expression == ' ' || *expression == '\t')) 
		expression++;
	return expression;
}
char* number_(char* expression, double& result) {
	char* end = nullptr;
	result = strtod(expression, &end);
	if (end == expression) {
		return nullptr;
	}
	else 
		return end;
}

char* factorial_(char* expression, double& result) {
	char* next = expression;

	next = subexpression(expression, result);

	if (next) {
		if (*next == '!') {
			result = factorial_operation(result);
			expression = ++next;
		}
	}
	else 
		ParserError("Operand missing", expression, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
	return next;
}
char* pow_(char* expression, double& result) {
	double _lhs;
	char* next = expression;

	next = unary_minus_(expression, result);
	expression = next;

	while (next) {
		if (*next == '^') {
			expression = ++next;
			next = factorial_(expression, _lhs);
			result = pow_operation(result, _lhs);
			expression = next;
		}
		else
			break;
	}
	return expression;
}
char* factors_(char* expression, double& result) {
	double _lhs;
	char* next = expression;

	next = pow_(expression, result);
	expression = next;

	while (next) {
		bool brk = false;
		switch (*next) {
		case '*':
			expression = ++next;
			next = pow_(expression, _lhs);
			result *= _lhs;
			expression = next;
			break;
		case '/':
			expression = ++next;
			next = pow_(expression, _lhs);
			result /= _lhs;
			expression = next;
			break;
		default:
			brk = true;
		}
		if (brk) break;
	}
	return expression;
}
/*
 * adding and substracting
 */
char* addings_(char* expression, double& result) {
	double _lhs;
	char* next = expression;

	next = factors_(expression, result);
	expression = next;

	while (next) {
		bool brk = false;
		switch (*next) {
		case '+':
			next = factors_(++expression, _lhs);
			result += _lhs;
			expression = next;
			break;
		case '-':
			next = factors_(++expression, _lhs);
			result -= _lhs;
			expression = next;
			break;
		default:
			brk = true;
		}
		if (brk) break;
	}
	return expression;
}
double execute(char * expression)
{
	double result;
	char* tmp = addings_(expression, result);
	if (!tmp || *tmp != 0) {
		std::string error = "Unexpected char '";
		error.push_back(*tmp);
		error.push_back('\'');
		ParserError(error.c_str(), tmp, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);
	}
	else
		return result;
}
