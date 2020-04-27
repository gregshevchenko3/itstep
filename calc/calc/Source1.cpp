//#ifdef version2
#include <iostream>
#include <cstring>
#include <cmath> // for std::pow() in linux

char* get_line();
/**
 * ������� �� ���������.
 *
 * expr - ��� �������� � �������� - �������� �� ������, �� ������ �����. � ������ - �������� �� ������, �� 
 * ���������� � ���������� ���������, � �� �, ��������� � ������� ���������� ��������� ����� ����������� ���������,
 * ���� ���������� ��� ���������, ��������� � ������ ����������.
 *
 * error_code - �������� �� �����, � ��� ����������� ����������� ��� �������.
 *
 * lhs - ��� �������� � �������� �� �� �����, ���� - nullptr. � ������, ����� ��� ��������� ���������� ��������� ��������� 
 * ���� ��������� �������� ����������, �� nullptr ���������� �������� ����������.
 *
 * unary - ��� �������� � �������� - �� �� �����, ���� ������ true. � ������, ����� ���� �������� ������� ���� ��������� (�������, �� �������)
 *
 * prev_operator_priority - ��� �������� � �������� - �� �� �����, ���� ������ 0. � ������ - �������� ������������ ���������.
 */
long long parse(char** expr, long *error_code, long long* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);
/**
 * ��'���� � ��� strtoll() �� strtod(). ���� �� �������� ������� �������������� str, �� ����� - ������� false.
 * ���� ������� �������������� str, �� ���� �����, ������� true, is_integer ���������� � true, � ����� � ll_num,
 * ���� ������� �������������� str, �� ����� �����, ������� true, is_integer ���������� � false, � ����� � d_num.
 */
bool strtonum(char* str, long& is_integer, long long& ll_num, double& d_num);
//double parse(char** expr, double* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);

// ����������� ��� �������.
const char* const error_msg[] = {
	"No Error",								// error_code = 0
	"",										// error_code = 1
	"Invalid unary operator",				// error_code = 2
	"Invalid binary operator",				// error_code = 3
	"Division by zero",						// error_code = 4
	"Missing \"(\"",						// error_code = 5
	"Missing \")\"",						// error_code = 6
	"Not a number",							// error_code = 7
	"Operation not supported",				// error_code = 8
};


// ������ ���������
const char* const tokens =  "+-*/%^()!"; // �� ���������
const char* const prefix_tokens = "-(";  // ҳ���� ��������
const char* const postfix_tokens = "!)"; // ҳ���� ���������
/**
 * ������ ��������� ���������. ����� ������ � ������ ������� �������� ���������� ��������� � ������ tokens. 
 * ���� � ��������� ���� ������/������ �����, �� ��������� ������ � ������ priority (upriority - ��� �������) == '0'
 * ��� ���������, ���� �������� ��������� ����� ������ ��������� == '-'. ������ �� �쳺 ����������� �������� ���������,
 * ���� ���� �������� � ��� ���� �����.
 */
const char* const priority =  "112223000";
const char* const upriority = "0400006-5";





int main() //for linux`s g++
{
	std::cout << "Simple console calc. For exit - press \"Ctrl+C\"" << std::endl;
	while (1){
		std::cout << "calc> ";
		char* line = get_line();
		char* tmp = line;

		long err_code = 0;
		long long result = parse(&tmp, &err_code, (long long*)nullptr);
		if (err_code > 1) {
			std::cout << "Error: " << error_msg[err_code] << std::endl;
		}
		else if(err_code != 1){
			std::cout << result << std::endl;
		}
		tmp = nullptr;
		delete[] line;
	}
	return 0;
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
long long parse(char** expr, long *error_code, long long* lhs, bool unary, char prev_operator_priority)
{
	if (**expr == 0) {
		*error_code = 1;
		return 0;
	}
	char* tmp = new char[strlen(*expr) + 1];
	strcpy(tmp, *expr);
	// �������� �� ����� ��� ������ �����.
	char* it = strtok(tmp, tokens); 
	// ��� �������� - ��������� ����������. ��������� ����� � expr ���� ���������� � ������� ������� � ���������,
	// ���� ���� ���������� � �����, �� �� ������, �� ��� �������� == 0.
	char this_operator = ((*tmp < '0' || *tmp > '9')) ? *tmp : 0;
	// �������� �� �������� ��������, ���� �������� ������ �� ��� ����������. �������� �� _next_operator
	// ���������� � parse().
	char* _next_operator = *expr + 1;
	// ���� ��������� �������� ���� ����������, �� ������� - is_unary_next_operator == true;
	bool is_unary_next_operator = false;
	// ���� ������� - �� ��������� (it == nullptr), �� ��� ��������� ������� ��������� ��� ����������� - ����� ������.
	// ����� ����� ��������� ��������� ����������� �� *expr+1.
	if (it)
		_next_operator = (it - tmp > 1) ? _next_operator : *expr + (it - tmp + strlen(it));
	/**
	 * ����� ���������� ���������.
	 * 1) ���� �������� �������� - �� ������� ����������� (�� � ������ postfix_tokens), i ��������� �������� ���� ������ �� ��������
	 * ������������ - �������� �������� ���� ���������� �������.
	 *
	 * 2)���� ��������� �������� - � � ������ ����������� (postfix_tokens), � �� ��� ���� ������ ��������, ��� ����� ������,
	 * �� ��������� �������� ���� �������� �����������.
	 */
	if(*_next_operator) {
		is_unary_next_operator = !strchr(postfix_tokens, this_operator) && (it - tmp > 1);
		is_unary_next_operator = is_unary_next_operator || strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
	}

	// �������� ����� �� ���������� ���������.
	char this_operator_priority = 0, next_operator_priority = 0;
	if (*_next_operator) {
		this_operator_priority = *((unary) ? (upriority + (strchr(tokens, this_operator) - tokens)) : (priority + (strchr(tokens, this_operator) - tokens)));
		next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
	}
	long long expr_rhs = 0, _rhs = 0;
	if(it)
		_rhs = strtoll(it, nullptr, 0);
	else 
		if (!unary) {
			*error_code = 3; // Unknown binary operator;
			return 0;
		}
	delete[] tmp;
	/**
	 * ���� �������� ���������� ��������� ����� �� �������� ��������� ���������, �� �������� �������� ��������� ��������,
	 * � ��� ���� ���������� ���.
	 */
	if (next_operator_priority != '-' && next_operator_priority > this_operator_priority && this_operator_priority != '-') {
		expr_rhs = _rhs;
		while (next_operator_priority > this_operator_priority && this_operator) {
			_rhs = parse(&_next_operator, error_code, &expr_rhs, is_unary_next_operator, this_operator_priority);
			expr_rhs = _rhs;
			is_unary_next_operator = strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
			next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
		}
		
	}
	else
		expr_rhs = _rhs;
	// ��������� ��������� ���������
	static long open_breket_count = 0;
	if (unary) {
		switch (this_operator)
		{
		case 0:
			break;
		case '-':
			// ������� ����
			_rhs = -expr_rhs;
			break;
		/*case '!':
			// �������� ��� �������.
			if (lhs) _rhs = factorial(*lhs);
			break; */
		case '(':
			// ������� ��������		
			open_breket_count++;
			_rhs = parse(&_next_operator, error_code, &_rhs, is_unary_next_operator, this_operator_priority);
			is_unary_next_operator = false;
			*expr = _next_operator;
			break;
		case ')':
			open_breket_count--;
			// ʳ���� ��������.
			if (lhs) _rhs = *lhs;
			if (open_breket_count < 0) {
				open_breket_count = 0;
				*error_code = 5;
				return 0;
			}
			break;
		default:
			*error_code = 2; // Unknown binary operator.
		}
	} else
		switch (this_operator)
		{
		case '-':
			// ³�������. 
			_rhs = *lhs - expr_rhs;
			break;
		case '+':
			// ���������. 
			_rhs = *lhs + expr_rhs;
			break;
		case '*':
			// ��������. 
			_rhs = *lhs * expr_rhs;
			break;
		case '/':
			// ĳ�����.
			if (!expr_rhs) {
				*error_code = 4;
				return 0;
			}
			_rhs = *lhs / expr_rhs;
			break;
		case '%':
			// ������� �� ������. 
			_rhs = *lhs % expr_rhs;
			break;
		case '^':
			// ϳ�������� �� �������. 
			_rhs = std::pow(*lhs, expr_rhs);
			break;
		default:
			*error_code = 3; // Unknown binary operator.
		}
	/**
	 * ���� �������� ������������ ��������� ������, �� �������� ���������, �� ������� �������� ����������� ����������,
	 * ��������� ��������� ���������, � �������� ����� ����������� � expr
	 */
	if (prev_operator_priority && prev_operator_priority < this_operator_priority || this_operator_priority == '-') {
		*expr = _next_operator;
		return _rhs;
	}
	if (open_breket_count > 0 && !*_next_operator) {
		open_breket_count = 0;
		*error_code = 6;
		return 0;
	}
	/** ���������� ��������� ��������� */
	if (*_next_operator)
		_rhs = parse(&_next_operator, error_code, &_rhs, is_unary_next_operator, this_operator_priority);

	*expr = _next_operator;
	return _rhs;
}

bool strtonum(char * str, long & is_integer, long long & ll_num, double & d_num)
{

	return false;
}

double parse(char** expr, double* lhs, bool unary, char prev_operator_priority)
{
	char* tmp = new char[strlen(*expr) + 1];
	strcpy(tmp, *expr);
	// �������� �� ����� ��� ������ �����.
	char* it = strtok(tmp, tokens);
	// ��� �������� - ��������� ����������.
	/*
	 * ������ ����� � ���� �������, ������� strtok() �� ������ 0-� ������, �� ������� ������ �� ������� � ������ tokens;
	 * �����  ������� ���������� ��� linux, ��� ������������ � �����.
	 */
	char this_operator = ((*tmp < '0' || *tmp > '9')) ? *tmp : 0;
	// �������� �� �������� ��������,
	char* _next_operator = *expr + 1;
	bool is_unary_next_operator = false;
	if (it)
		_next_operator = (it - tmp > 1) ? _next_operator : *expr + (it - tmp + strlen(it));
	/**
	 * ����� ���������� ���������.
	 * 1) ���� �������� �������� - �� ������� ����������� (�� � ������ postfix_tokens), i ��������� �������� ���� ������ �� ��������
	 * ������������ - �������� �������� ���� ���������� �������.
	 *
	 * 2)���� ��������� �������� - ������� � � � ������ ����������� (postfix_tokens), � �� ��� ���� ������ ��������, ��� ����� ������,
	 * �� ��������� �������� ���� �������� �����������.
	 */
	is_unary_next_operator = !strchr(postfix_tokens, this_operator) && strchr(prefix_tokens, *(*expr + 1));
	is_unary_next_operator = is_unary_next_operator || strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
	// �������� ����� �� ���������� ���������
	char this_operator_priority = *((unary) ? (upriority + (strchr(tokens, this_operator) - tokens)) : (priority + (strchr(tokens, this_operator) - tokens))),
		next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));

	double expr_rhs = 0, _rhs = 0;
	if (it)
		_rhs = strtod(it, nullptr);
	delete[] tmp;
	/**
	 * ���� �������� ���������� ��������� ����� �� �������� ��������� ���������, �� �������� �������� ��������� ��������,
	 * � ��� ���� ���������� ���.
	 */
	if (next_operator_priority != '-' && next_operator_priority > this_operator_priority && this_operator_priority != '-') {
		expr_rhs = _rhs;
		while (next_operator_priority > this_operator_priority && this_operator) {
			_rhs = parse(&_next_operator, &expr_rhs, is_unary_next_operator, this_operator_priority);
			expr_rhs = _rhs;
			is_unary_next_operator = strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
			next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
		}

	}
	else
		expr_rhs = _rhs;
	// ��������� ��������� ���������
	if (unary) {
		switch (this_operator)
		{
		case '-':
			// ������� ����
			_rhs = -expr_rhs;
			break;
			/*case '!':
				// �������� ��� �������.
				if (lhs) _rhs = factorial(*lhs);
				break; */
		case ')':
			// ʳ���� ��������.
			if (lhs) _rhs = *lhs;
			break;
		case '(':
			// ������� ��������
		{
			_rhs = parse(&_next_operator, &_rhs, is_unary_next_operator, this_operator_priority);
			is_unary_next_operator = false;
			*expr = _next_operator;
		}
		}
	}
	else
		switch (this_operator)
		{
		case '-':
			// ³�������. 
			_rhs = *lhs - expr_rhs;
			break;
		case '+':
			// ���������. 
			_rhs = *lhs + expr_rhs;
			break;
		case '*':
			// ��������. 
			_rhs = *lhs * expr_rhs;
			break;
		case '/':
			// ĳ�����.
			_rhs = *lhs / expr_rhs;
			break;
		case '%': {
			long long ll_tmp_lhs = *lhs, ll_tmp_rhs = expr_rhs;
			if (ll_tmp_lhs == *lhs && ll_tmp_rhs == expr_rhs) {
				// ������� �� ������. 
				_rhs = ll_tmp_lhs % ll_tmp_rhs;
			}
			break;
		}
		case '^':
			// ϳ�������� �� �������. 
			_rhs = std::pow(*lhs, expr_rhs);
			break;
		}
	/**
	 * ���� �������� ������������ ��������� ������, �� �������� ���������, �� ������� �������� ����������� ����������,
	 * ��������� ��������� ���������, � �������� ����� ����������� � expr
	 */
	if (prev_operator_priority && prev_operator_priority < this_operator_priority || this_operator_priority == '-') {
		*expr = _next_operator;
		return _rhs;
	}
	/** ���������� ��������� ��������� */
	if (*_next_operator)
		_rhs = parse(&_next_operator, &_rhs, is_unary_next_operator, this_operator_priority);
	*expr = _next_operator;
	return _rhs;
}
//#endif
