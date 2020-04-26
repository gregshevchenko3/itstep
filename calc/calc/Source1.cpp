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
 * error_code - Вказівник на змінну, в яку збережеться повідомлення про помилку.
 *
 * lhs - При викликові з програми не має сенсу, тому - nullptr. В рекурсії, через цей параметер передається результат виконання 
 * лівих операторів бінарному операторові, та nullptr передається унарному операторові.
 *
 * unary - При викликові з програми - не має сенсу, тому завжди true. В рекурсії, вказує який аргумент повинен бути виконаний (унарний, чи бінарний)
 *
 * prev_operator_priority - При викликові з програми - не має сенсу, тому завжди 0. В рекурсії - пріоритет попереднього оператора.
 */
long long parse(char** expr, long *error_code, long long* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);
//double parse(char** expr, double* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);

// Повідомлення про помилку.
const char* const error_msg[] = {
	"No Error",
	"Invalid unary operator",
	"Invalid binary operator"
};


// Масив операторів
const char* const tokens =  "+-*/%^()!";
const char* const prefix_tokens = "-(";
const char* const postfix_tokens = "!)";
// Масив пріорітитетів операторів.
const char* const priority =  "112223000";
const char* const upriority = "0400006-5";





void main()
{
	std::cout << "Simple console calc. For exit - press \"Ctrl+C\"" << std::endl;
	while (1){
		std::cout << "calc> ";
		char* line = get_line();
		char* tmp = line;

		long err_code = 0;
		long long result = parse(&tmp, &err_code, (long long*)nullptr);
		if (err_code) {
			std::cout << "Error: " << error_msg[err_code] << std::endl;
		}
		else {
			std::cout << result << std::endl;
		}
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
long long parse(char** expr, long *error_code, long long* lhs, bool unary, char prev_operator_priority)
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

	bool is_this_operator_have_postfix_form = strchr(postfix_tokens, this_operator),
		is_next_operator_have_postfix_form = strchr(postfix_tokens, *_next_operator);

	is_unary_next_operator = !is_this_operator_have_postfix_form && (it - tmp > 1);
	is_unary_next_operator = is_unary_next_operator || is_next_operator_have_postfix_form && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);


	

	// пріоритет цього та наступного операторів
	char this_operator_priority = *((unary) ? (upriority + (strchr(tokens, this_operator) - tokens)) : (priority + (strchr(tokens, this_operator) - tokens))),
		next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
	long long expr_rhs = 0, _rhs = 0;
	if(it)
		_rhs = strtoll(it, nullptr, 0);
	else 
		if (!unary) {
			*error_code = 2; // Unknown binary operator;
			return 0;
		}
	delete[] tmp;
	/**
	 * Якщо пріоритет наступного оператора вищий за пріоритет поточного оператора, то спочатку виконати наступний оператор,
	 * а вже потім виконувати цей.
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
	// Виконання поточного оператора
	if (unary) {
		switch (this_operator)
		{
		case 0:
			break;
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
				_rhs = parse(&_next_operator, error_code, &_rhs, is_unary_next_operator, this_operator_priority);
				is_unary_next_operator = false;
				*expr = _next_operator;
			}
			break;
		default:
			*error_code = 1; // Unknown unary operator.
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
		default:
			*error_code = 2; // Unknown binary operator.
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
		_rhs = parse(&_next_operator, error_code, &_rhs, is_unary_next_operator, this_operator_priority);
	*expr = _next_operator;
	return _rhs;
}

double parse(char** expr, double* lhs, bool unary, char prev_operator_priority)
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
	char this_operator_priority = *((unary) ? (upriority + (strchr(tokens, this_operator) - tokens)) : (priority + (strchr(tokens, this_operator) - tokens))),
		next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
	double expr_rhs = 0, _rhs = 0;
	if (it)
		_rhs = strtod(it, nullptr);
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
			next_operator_priority = *((is_unary_next_operator) ? (upriority + (strchr(tokens, *_next_operator) - tokens)) : (priority + (strchr(tokens, *_next_operator) - tokens)));
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
	}
	else
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
		case '%': {
			long long ll_tmp_lhs = *lhs, ll_tmp_rhs = expr_rhs;
			if (ll_tmp_lhs == *lhs && ll_tmp_rhs == expr_rhs) {
				// Залишок від ділення. 
				_rhs = ll_tmp_lhs % ll_tmp_rhs;
			}
			break;
		}
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
	*expr = _next_operator;
	return _rhs;
}

