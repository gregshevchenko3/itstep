#include <iostream>
#include <cstring>


char* get_line();
long long parse(char** line, bool& ErrorFlag);
long long parse_private(char** line, bool& ErrorFlag);

inline long long NANError(char*& tmp, char** line, bool& ErrorFlag);
inline long long InputError(char*& tmp, bool& ErrorFlag);

bool scaling_operator(long long& lhs, char*& op, bool& ErrorFlag);
bool pow_operator(long long& lhs, char*& op, bool& ErrorFlag);


void main()
{
	std::cout << "Simple console calc. For exit press Ctrl+C" << std::endl;
	while (true) {
		std::cout << "calc> ";
		char* line = get_line();
		bool is_error = 0;
		long long result = parse(&line, is_error);
		if(!is_error)
			std::cout << result << std::endl;
		delete[] line;
	}
}

char* get_line()
{
	std::size_t count = 1;
	char *line = new char[count];
	while ('\n' != (*(line + count - 1) = std::cin.get()))
	{
		char* tmp = line;
		line = new char[count + 1];
		std::copy(tmp, tmp + count++, line);
		delete[] tmp;
		tmp = nullptr;

	};
	line[count - 1] = 0;
	return line;
}
long long parse(char** line, bool& ErrorFlag) {
	char* tmpline = *line;
	return parse_private(&tmpline, ErrorFlag);
}
long long parse_private(char** line, bool& ErrorFlag) {
	static const char *tokens = "-+*/%^";
	char* tmp = new char[strlen(*line) + 1];
	strcpy(tmp, *line);

	long long lhs = 0;

	char* next_operator = nullptr, this_operator = 0;

	char *it = strtok(tmp, tokens);
	if (!it) return InputError(tmp, ErrorFlag);

	if (*tmp < '0' || *tmp > '9') this_operator = *tmp;

	if (this_operator == '+' || this_operator == '-' || this_operator == 0) {
		// Додавання та віднімання
		if (it - tmp > 1) {
			next_operator = (*line + 1);
			if (*next_operator == '-' && this_operator == '+') // lhs + -3 Можливо, все інше - ні.
			{
				delete[] tmp;
				return parse_private(&next_operator, ErrorFlag);
			}
			else
				return InputError(tmp, ErrorFlag);
		}
		next_operator = *line + (it - tmp) + strlen(it);
		char* nan;
		lhs = strtoll(tmp, &nan, 0);
		if (nan == tmp) return NANError(tmp, line, ErrorFlag);
		delete[] tmp;
		// Спочатку виконати оператори, що знаходяться правіше.
		while (scaling_operator(lhs, next_operator, ErrorFlag) || pow_operator(lhs, next_operator, ErrorFlag));
		if (*next_operator == 0)
			// далі кінець виразу
			return lhs;
		else
			// ще не кінець
			return lhs + parse_private(&next_operator, ErrorFlag);
	}
	else if (this_operator == '*' || this_operator == '/' || this_operator == '%' || this_operator == '^') {
		// Множення, ділення, залишок від ділення, та піднесення до степеню
		if (it - tmp > 1) {
			next_operator = (*line + 1);
			if (*next_operator == '-') // lhs*-3 Можливо, все інше - ні.
			{
				delete[] tmp;
				return parse_private(&next_operator, ErrorFlag);
			}
			else
				return InputError(tmp, ErrorFlag);
		}
		next_operator = *line + (it - tmp) + strlen(it);
		char* nan;
		lhs = strtoll(it, &nan, 0);
		if (nan == it) return NANError(tmp, line, ErrorFlag);
		delete[] tmp;
		// Пріоритет операції піднесення степення вищий, ніж у множення, ділення, та модулю, 
		if (*next_operator == '^') {
			pow_operator(lhs, next_operator, ErrorFlag);
		}
		*line = next_operator;
		return lhs;
	}
	return InputError(tmp, ErrorFlag);
}

inline long long InputError(char*& tmp, bool& ErrorFlag) {
	ErrorFlag = true;
	delete[] tmp;
	return 0;
}
inline long long NANError(char*& tmp, char** line, bool& ErrorFlag) {
	ErrorFlag = true;
	*line += strlen(*line);
	delete[] tmp;
	return 0;
}

bool scaling_operator(long long& lhs, char*& op, bool& ErrorFlag)
{
	bool result = true;
	long long rhs = 0;
	switch (*op) {
	case '*':
		lhs *= parse_private(&op, ErrorFlag);
		break;
	case '/':
		rhs = parse_private(&op, ErrorFlag);
		if (rhs == 0 || ErrorFlag) {
			ErrorFlag = true;
			lhs = 0;
			op = op + strlen(op);
			return false;
		}
		lhs /= rhs;
		break;
	case '%':
		rhs = parse_private(&op, ErrorFlag);
		if (rhs == 0 || ErrorFlag) {
			ErrorFlag = true;
			lhs = 0;
			op = op + strlen(op);
			return false;
		}
		lhs %= rhs;
		break;
	default:
		result = false;
	}
	return result && !ErrorFlag;
}
bool pow_operator(long long& lhs, char*& op, bool& ErrorFlag) {
	if (*op == '^') {
		lhs = std::pow(lhs, parse_private(&op, ErrorFlag));
		return true && !ErrorFlag;
	}
	return false && !ErrorFlag;
}
