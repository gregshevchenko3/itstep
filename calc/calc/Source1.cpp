#include <iostream>
#include <cstring>

char* get_line();
/**
 * Парсинг та виконання.
 *
 * expr - При викликові з програми - вказівник на строку, що містить вираз. В рекурсії - вказівник на строку, що 
 * починається з наступного оператора, в неї ж, оператори з високим пріоритетом зберігають точку продовження виконання,
 * коли повертають свій результат, оператору з нижчим пріоритетом.
 *
 * lhs - При викликові з програми не має сенсу, тому - nullptr. В рекурсії, через цей параметер передається результат виконання 
 * лівих операторів бінарному операторові, та nullptr передається унарному операторові.
 *
 * prev_operator_priority - При викликові з програми - не має сенсу, тому завжди 0. В рекурсії - пріоритет попереднього оператора.
 */
long long parse(char** expr, long long* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);
char get_operator_priority(char* Operator, bool u = false);

// Масив операторів
const char* const tokens =  "+-*/%^()!";
const char* const prefix_tokens = "-(";
const char* const postfix_tokens = "!)";
// Масив пріорітитетів операторів.
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
	// Вказівник на стоку яка містить число.
	char* it = strtok(tmp, tokens); 
	// цей оператор - символьне позначення.
	/*
	 * Моживо тільки в моєму випадку, функція strtok() не замінює 0-й символ, що співпадає зодним із символів в шаблоні tokens;
	 * пізніше  спробую перезібрати для linux, щоб переконатися в цьому.
	 */
	char this_operator = ((*tmp < '0' || *tmp > '9')) ? *tmp : 0;
	// Вказівник на наступий оператор,
	char* _next_operator = *expr + 1;
	bool is_unary_next_operator = false;
	if (it) 
		_next_operator = (it - tmp > 1) ? _next_operator : *expr + (it - tmp + strlen(it));
	/**
	 * Аналіз наступного оператора.
	 * 1) Якщо поточний оператор - не унарний постфіксний (не в списку postfix_tokens), i наступний оператор слідує відразу за поточним
	 * операторотом - наступий оператор буде префіксним унарним.
	 *
	 * 2)Якщо наступний оператор - унарний і є в списку постфіксних (postfix_tokens), і за ним слідує якийсь оператор, або кінець строки,
	 * то наступний оператор буде унарнним постфіксним.
	 */
	is_unary_next_operator = !strchr(postfix_tokens, this_operator) && strchr(prefix_tokens, *(*expr + 1));
	is_unary_next_operator = is_unary_next_operator || strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
	// пріоритет цього та наступного операторів
	char this_operator_priority =  get_operator_priority(&this_operator, unary),   
		next_operator_priority = get_operator_priority(_next_operator, is_unary_next_operator);
	long long expr_rhs = 0, _rhs = 0;
	if(it)
		_rhs = strtoll(it, nullptr, 0);
	delete[] tmp;
	/**
	 * Якщо пріоритет наступного оператора вищий за пріоритет поточного оператора, то спочатку виконати наступний оператор,
	 * а вже потім виконувати цей.
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
	// Виконання поточного оператора
	if (unary) {
		switch (this_operator)
		{
		case '-':
			// Унарний мінус
			_rhs = -expr_rhs;
			break;
		/*case '!':
			// Факторіал лівої частини.
			if (lhs) _rhs = factorial(*lhs);
			break; */
		case ')': 
			// Кінець підвиразу.
			if (lhs) _rhs = *lhs;
			break;
		case '(':
			// Початок підвиразу
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
			// Віднімання. 
			_rhs = *lhs - expr_rhs;
			break;
		case '+':
			// Додавання. 
			_rhs = *lhs + expr_rhs;
			break;
		case '*':
			// Множення. 
			_rhs = *lhs * expr_rhs;
			break;
		case '/':
			// Ділення.
			_rhs = *lhs / expr_rhs;
			break;
		case '%':
			// Залишок від ділення. 
			_rhs = *lhs % expr_rhs;
			break;
		case '^':
			// Піднесення до степеня. 
			_rhs = std::pow(*lhs, expr_rhs);
			break;
		}
	/**
	 * Якщо пріоритет попереднього оператора менший, за пріоритет поточного, то потрібно передати попередному операторові,
	 * результат поточного оператора, і зберегти точку продовження в expr
	 */
	if (prev_operator_priority && prev_operator_priority < this_operator_priority || this_operator_priority == '-') {
		*expr = _next_operator;
		return _rhs;
	}
	/** Продовжуємо виконання операторів */
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


