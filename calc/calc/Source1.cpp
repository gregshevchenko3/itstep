#include <iostream>
#include <cstring>

char* get_line();
/**
 * ������� �� ���������.
 *
 * expr - ��� �������� � �������� - �������� �� ������, �� ������ �����. � ������ - �������� �� ������, �� 
 * ���������� � ���������� ���������, � �� �, ��������� � ������� ���������� ��������� ����� ����������� ���������,
 * ���� ���������� ��� ���������, ��������� � ������ ����������.
 *
 * lhs - ��� �������� � �������� �� �� �����, ���� - nullptr. � ������, ����� ��� ��������� ���������� ��������� ��������� 
 * ���� ��������� �������� ����������, �� nullptr ���������� �������� ����������.
 *
 * prev_operator_priority - ��� �������� � �������� - �� �� �����, ���� ������ 0. � ������ - �������� ������������ ���������.
 */
long long parse(char** expr, long long* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);
char get_operator_priority(char* Operator, bool u = false);

// ����� ���������
const char* const tokens =  "+-*/%^()!";
const char* const prefix_tokens = "-(";
const char* const postfix_tokens = "!)";
// ����� ���������� ���������.
const char* const priority =  "112223000";
const char* const upriority = "0400006-5";





void main()
{
	while (1){
		char* line = get_line();
		char* tmp = line;
		std::cout << parse(&tmp, nullptr) << std::endl;
		tmp = nullptr;
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
long long parse(char** expr, long long* lhs, bool unary, char prev_operator_priority)
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
	char this_operator_priority =  get_operator_priority(&this_operator, unary),   
		next_operator_priority = get_operator_priority(_next_operator, is_unary_next_operator);
	long long expr_rhs = 0, _rhs = 0;
	if(it)
		_rhs = strtoll(it, nullptr, 0);
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
			next_operator_priority = get_operator_priority(_next_operator, is_unary_next_operator);
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
	return _rhs;
}
char get_operator_priority(char* Operator, bool u)
{
	char* operators = (char*)tokens;
	while (*Operator != *operators && *operators)
		operators++;
	if (u) {
		return *(upriority + (operators - tokens));
	}
	else {
		return *(priority + (operators - tokens));
	}
}


