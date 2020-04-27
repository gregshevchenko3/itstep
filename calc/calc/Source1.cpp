//#ifdef version2
#include <iostream>
#include <cstring>
#include <cmath> // for std::pow() in linux

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
/**
 * Об'єднує в собі strtoll() та strtod(). Якщо не виходить повністю інтерпретувати str, як число - повертає false.
 * якщо вдається інтерпретувати str, як ціле число, повертає true, is_integer встановлює в true, а число в ll_num,
 * якщо вдається інтерпретувати str, як дійсне число, повертає true, is_integer встановлює в false, а число в d_num.
 */
bool strtonum(char* str, long& is_integer, long long& ll_num, double& d_num);
//double parse(char** expr, double* lhs = nullptr, bool unary = true, char prev_operator_priority = 0);

// Повідомлення про помилку.
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


// Строки операторів
const char* const tokens =  "+-*/%^()!"; // Всі оператори
const char* const prefix_tokens = "-(";  // Тільки префіксні
const char* const postfix_tokens = "!)"; // Тільки постфіксні
/**
 * Строки пріоритетів операторів. Кожен символ в строці позначає пріоритет відповідного оператора в строці tokens. 
 * Якщо в оператора немає бінарної/унарної форми, то відповідний символ в строці priority (upriority - для унарних) == '0'
 * Для операторів, яким пріоритет назначити важко символ пріоритету == '-'. Парсер не вміє обчислювати пріоритет операторів,
 * тому бере пріоритет з цих двох строк.
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
	// Вказівник на стоку яка містить число.
	char* it = strtok(tmp, tokens); 
	// Цей оператор - символьне позначення. Переданий вираз в expr буде починатися в більшості випадків з оператора,
	// якщо буде починатися з числа, то це означає, що цей оператор == 0.
	char this_operator = ((*tmp < '0' || *tmp > '9')) ? *tmp : 0;
	// Вказівник на наступий оператор, Може слідувати відразу за цим оператором. Вказівник на _next_operator
	// передається в parse().
	char* _next_operator = *expr + 1;
	// Якщо наступний оператор буде визначений, як унарний - is_unary_next_operator == true;
	bool is_unary_next_operator = false;
	// Якщо операнд - не визначено (it == nullptr), то далі слідуєчерез декілька операторів або непоймичого - кінець виразу.
	// Таким чином Наступний опператор визначається як *expr+1.
	if (it)
		_next_operator = (it - tmp > 1) ? _next_operator : *expr + (it - tmp + strlen(it));
	/**
	 * Аналіз наступного оператора.
	 * 1) Якщо поточний оператор - не унарний постфіксний (не в списку postfix_tokens), i наступний оператор слідує відразу за поточним
	 * операторотом - наступий оператор буде префіксним унарним.
	 *
	 * 2)Якщо наступний оператор - є в списку постфіксних (postfix_tokens), і за ним слідує якийсь оператор, або кінець строки,
	 * то наступний оператор буде унарнним постфіксним.
	 */
	if(*_next_operator) {
		is_unary_next_operator = !strchr(postfix_tokens, this_operator) && (it - tmp > 1);
		is_unary_next_operator = is_unary_next_operator || strchr(postfix_tokens, *_next_operator) && (strchr(tokens, *(_next_operator + 1)) || *(_next_operator + 1) == 0);
	}

	// пріоритет цього та наступного операторів.
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
	static long open_breket_count = 0;
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
		case '(':
			// Початок підвиразу		
			open_breket_count++;
			_rhs = parse(&_next_operator, error_code, &_rhs, is_unary_next_operator, this_operator_priority);
			is_unary_next_operator = false;
			*expr = _next_operator;
			break;
		case ')':
			open_breket_count--;
			// Кінець підвиразу.
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
			if (!expr_rhs) {
				*error_code = 4;
				return 0;
			}
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
			*error_code = 3; // Unknown binary operator.
		}
	/**
	 * Якщо пріоритет попереднього оператора менший, за пріоритет поточного, то потрібно передати попередному операторові,
	 * результат поточного оператора, і зберегти точку продовження в expr
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
	/** Продовжуємо виконання операторів */
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
//#endif
