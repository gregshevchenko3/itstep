#include <iostream>
#include <cstring>
#include <cerrno>
#include <cmath> // for std::pow() in linux


union numeric {
	double d;
	long long ll;
};


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
numeric parse(char** expr, bool& is_integral_result, long &error_code, numeric* lhs = nullptr, bool is_int_lhs = true, bool unary = true, char prev_operator_priority = 0);
/**
 * Об'єднує в собі strtoll() та strtod(). Якщо не виходить повністю інтерпретувати str, як число - повертає false.
 * якщо вдається інтерпретувати str, як ціле число, повертає true, is_integer встановлює в true, а число в ll_num,
 * якщо вдається інтерпретувати str, як дійсне число, повертає true, is_integer встановлює в false, а число в d_num.
 */
bool strtonum(char* str, bool& is_integer, numeric& num);

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
		bool is_result_integral = true;
		long err_code = 0;
		numeric result = parse(&tmp, is_result_integral, err_code);
		if (err_code > 1) {
			std::cout << "Error: " << error_msg[err_code] << std::endl;
		}
		else if(err_code != 1){
			std::cout << ((is_result_integral)?result.ll:result.d )<< std::endl;
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
numeric parse(char** expr, bool& is_integral_result, long &error_code, numeric* lhs, bool is_int_lhs, bool unary, char prev_operator_priority)
{
	if (**expr == 0) {
		error_code = 1;
		numeric ll;
		ll.ll = 0;
		return ll;
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
	
	numeric expr_rhs, _rhs; 
	bool is_int_rhs = true;
	expr_rhs.ll = 0;
	_rhs.ll = 0;
	if (it) {
		if (!strtonum(it, is_int_rhs, _rhs)) {
			error_code = 7; // NaN
			_rhs.ll = 0;
			return _rhs;
		}
	}
	else 
		if (!unary) {
			error_code = 3; // Unknown binary operator;
			_rhs.ll = 0;
			return _rhs;
		}
	delete[] tmp;
	/**
	 * Якщо пріоритет наступного оператора вищий за пріоритет поточного оператора, то спочатку виконати наступний оператор,
	 * а вже потім виконувати цей.
	 */
	if (next_operator_priority != '-' && next_operator_priority > this_operator_priority && this_operator_priority != '-') {
		expr_rhs = _rhs;
		while (next_operator_priority > this_operator_priority && this_operator) {
			_rhs = parse(&_next_operator, is_int_rhs, error_code, &expr_rhs, is_int_rhs, is_unary_next_operator, this_operator_priority);
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
			if(is_int_rhs)
				_rhs.ll = -expr_rhs.ll;
			else
				_rhs.d = -expr_rhs.d;
			break;
		/*case '!':
			// Факторіал лівої частини.
			if (lhs) _rhs = factorial(*lhs);
			break; */
		case '(':
			// Початок підвиразу		
			open_breket_count++;
			_rhs = parse(&_next_operator, is_int_rhs, error_code, &_rhs, is_int_rhs, is_unary_next_operator, this_operator_priority);;
			is_unary_next_operator = false;
			is_integral_result = is_int_rhs;
			*expr = _next_operator;
			break;
		case ')':
			open_breket_count--;
			// Кінець підвиразу.
			if (lhs) {
				_rhs = *lhs;
				is_int_rhs = is_int_lhs;
			}
			if (open_breket_count < 0) {
				open_breket_count = 0;
				error_code = 5;
				_rhs.ll = 0;
				return _rhs;
			}
			break;
		default:
			error_code = 2; // Unknown binary operator.
		}
	} else
		switch (this_operator)
		{
		case '-':
			// Віднімання. 
			if(is_int_rhs && is_int_lhs)
				_rhs.ll = lhs->ll - expr_rhs.ll;
			else {
				_rhs.d = ((is_int_lhs)?lhs->ll:lhs->d) - ((is_int_rhs)?expr_rhs.ll:expr_rhs.d);
				is_int_rhs = false;
			}
			break;
		case '+':
			// Додавання. 
			if (is_int_rhs && is_int_lhs)
				_rhs.ll = lhs->ll + expr_rhs.ll;
			else {
				_rhs.d = ((is_int_lhs) ? lhs->ll : lhs->d) + ((is_int_rhs) ? expr_rhs.ll : expr_rhs.d);
				is_int_rhs = false;
			}
			break;
		case '*':
			// Множення. 
			if (is_int_rhs && is_int_lhs)
				_rhs.ll = lhs->ll * expr_rhs.ll;
			else {
				_rhs.d = ((is_int_lhs) ? lhs->ll : lhs->d) * ((is_int_rhs) ? expr_rhs.ll : expr_rhs.d);
				is_int_rhs = false;
			}
			break;
		case '/':
			// Ділення.
			if (is_int_rhs && is_int_lhs) {
				if (expr_rhs.ll == 0) {
					error_code = 4;
					_rhs.ll = 0;
					return _rhs;
				}
				if (expr_rhs.ll >= lhs->ll)
				{
					_rhs.d = (double)lhs->ll / expr_rhs.ll;
					is_int_rhs = false;
				}
				else {
					_rhs.ll = lhs->ll / expr_rhs.ll;
					is_int_rhs = false;
				}
			}
			else {
				_rhs.d = ((is_int_lhs) ? lhs->ll : lhs->d ) / ((is_int_rhs) ? expr_rhs.ll : expr_rhs.d);
				is_int_rhs = false;
			}
			break;
		case '%':
			// Залишок від ділення. 
			if (is_int_rhs && is_int_lhs) {
				_rhs.ll = lhs->ll % expr_rhs.ll;
			}
			else {
				error_code = 8;
				_rhs.ll = 0;
				return _rhs;
			}
			break;
		case '^':
			// Піднесення до степеня. 
			lhs->d = (is_int_lhs) ? lhs->ll : lhs->d;
			expr_rhs.d = (is_int_rhs) ? expr_rhs.ll : expr_rhs.d;
			_rhs.d = std::pow(lhs->d, expr_rhs.d);
			is_int_rhs = false;
			break;
		default:
			error_code = 3; // Unknown binary operator.
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
		error_code = 6;
		_rhs.ll = 0;
		return _rhs;
	}
	/** Продовжуємо виконання операторів */
	if (*_next_operator)
		_rhs = parse(&_next_operator, is_int_rhs, error_code, &_rhs, is_int_rhs, is_unary_next_operator, this_operator_priority);

	*expr = _next_operator;
	is_integral_result = is_int_rhs;
	return _rhs;
}

bool strtonum(char * str, bool & is_integer, numeric& num)
{
	char* NaN = nullptr;
	num.ll = strtoll(str, &NaN, 0);
	is_integer = str + strlen(str) == NaN;
	if(!is_integer) num.d = strtod(str, &NaN);
	return str + strlen(str) == NaN;
}

