#ifdef Version0

#include <iostream>

#include "../../Array/Array/arrays.h"

// Все, що може зустрітися в строці
enum tok_type {
	Number, Plus, Minus, Div, Mod, Pow, Mul,
};
/** 
 * Операторипо пріоритетам від найвищого до найнижчого
 *   -		унарний мінус
 *	 ^		піднесення до степеня
 *  * / %   множення, ділення, модуль
 *   + -    додавання віднімання.
 */

// Читаю строку посимвольно, поки не буде введено Enter
const char* get_line();
// Парсинг числа. Повертається елемент, що буде додано до масиву tokens
long long* parse_num(char*& begin);
// парсинг оператора. Повертається елемент, що буде додано до масиву tokens
long long* parse_operator(char*& begin);
// Аналіз масиву tokens, в якому знаходяться всі числа та операції, які потрібно над числами виконати
long long** execute(long long** tokens, std::size_t& tok_counts);
// видалення масиву tokens.
void delete_tokens(long long** tokens, std::size_t tok_count);

void main()
{

	std::size_t count;
	const char* in = nullptr;

	std::size_t rows = 0, cols = 2;

	/* 
	 * масив rows * cols
	 * Кожний рядок в масиві описує знайдений об'єкт в строці.
	 * tokens[rows][0] - одне із значень з enum tok_type
	 * tokens[rows][1] - Якщо tokens[rows][0] == tok_type::Number - містить значення знайденого числа, інакше - 0.
	 */
	long long** tokens = nullptr; 

	do
	{
		if (in) delete[] in;
		std::cout << "calc> "; 
		in = get_line();
		char* tmp = (char*)in;
		// Розбиваю отриману строку, і заношу в масив tokens - числа та оператори, якщо строка має символи, що не можуть бути інтерпретованими, як 
		// число, або оператор, то виводжу повідомення про помилку, і видаляю масив.

		while (*tmp != 0)
		{
			char* prev = tmp;
			long long* t;
			if (t = parse_num(tmp)) {
				if (rows == 0) {
					tokens = new long long*[++rows] {};
					tokens[0] = new long long[cols];
					std::copy(t, t + cols, tokens[0]);
				}
				else push_row_back(tokens, rows, cols, t);
				delete[] t;
			} else 
				if (t = parse_operator(tmp)) {
					if (rows == 0) {
						tokens = new long long*[++rows]{};
						tokens[0] = new long long[cols];
						std::copy(t, t + cols, tokens[0]);
					}
					else push_row_back(tokens, rows, cols, t);
					delete[] t;
				}
				else if (*tmp != 0) {
					std::cerr << "Input error!" << std::endl;
					break;
				}
		}
		tmp = 0;
		if (tokens) {
			// Аналіз масиву tokens, і виконання операцій. Якщо аналіз пройде без помилок, то функція execute
			// поверне масив tokens, що складається з одного елементу (однієї пари)
			tokens = execute(tokens, rows);
			if (tokens) {
				// Якщо все впорядку - виводжу на екран результат
				std::cout << tokens[0][1] << std::endl;
				// Видаляю масив tokens;
#ifdef _DEBUG
				std::cout << "DEBUG: tokens count after execute: " << rows << std::endl;
#endif
				delete_tokens(tokens, rows);
				tokens = nullptr;
			}
			// Скидаю в 0 лічильник елементів
			rows = 0;
		}
	}
	while(strcmp("exit", in) != 0);
	if (in) delete[] in;
}

const char* get_line()
{
	std::size_t count = 1;
	char *line = new char[count];
	while('\n' != (*(line + count - 1) = std::cin.get()))
	{
		char* tmp = line;
		line = new char[count + 1];
		std::copy(tmp, tmp + count++, line);
		delete[] tmp;
		tmp = nullptr;

	};
	line[count-1] = 0;
	return line;
}
long long* parse_num(char *& begin)
{
	long long* result = nullptr;
	while (*begin != 0) {
		if (*begin >= '0' && *begin <= '9') {
			if (!result) {
				result = new long long[2];
				result[0] = tok_type::Number;
				result[1] = 0;
			}
			result[1] *= 10;
			result[1] += *begin++ - '0';
		}
		else
			break;
	}

	return result;
}
long long* parse_operator(char *& begin)
{
	long long* result = new long long[2];
	switch(*begin)
	{
	case '+': result[0] = tok_type::Plus; break;
	case '-': result[0] = tok_type::Minus; break;
	case '/': result[0] = tok_type::Div; break;
	case '%': result[0] = tok_type::Mod; break;
	case '^': result[0] = tok_type::Pow; break;
	case '*': result[0] =  tok_type::Mul; break;
	default:
		delete[] result;
		return nullptr;
	}
	result[1] = 0;
	begin++;
	return result;
}
void delete_tokens(long long** tokens, std::size_t tok_count) {
	for (int i = 0; i < tok_count; i++) {
		delete[] tokens[i];
	}
	delete[] tokens;
}


long long** execute_adds_and_subs(long long** tokens, std::size_t& tok_count) {
	std::size_t cols = 2;
	for (int i = 0; i < tok_count; i++)
	{
		if (tokens[i][0] == tok_type::Plus || tokens[i][0] == tok_type::Minus) {
			if (i > 0) {
				if (tokens[i + 1][0] == tok_type::Number && tokens[i - 1][0] == tok_type::Number) {
					switch (tokens[i][0])
					{
					case tok_type::Plus:
						tokens[i - 1][1] = tokens[i - 1][1] + tokens[i + 1][1];
						break;
					case tok_type::Minus:
						tokens[i - 1][1] = tokens[i - 1][1] - tokens[i + 1][1];
						break;
					}
					remove_row(tokens, tok_count, cols, i);
					remove_row(tokens, tok_count, cols, i);
					i = 0;
				}
				else {
					std::cerr << "Syntax error!" << std::endl;
					delete_tokens(tokens, tok_count);
					return nullptr;
				}
			}
			else {
				std::cerr << "Syntax error!" << std::endl;
				delete_tokens(tokens, tok_count);
				return nullptr;
			}
		}
	}
	return tokens;
}
long long** execute_factor(long long** tokens, std::size_t& tok_count) {
	std::size_t cols = 2;
	for (int i = 0; i < tok_count; i++)
	{
		if (tokens[i][0] == tok_type::Mul || tokens[i][0] == tok_type::Div || tokens[i][0] == tok_type::Mod) {
			if (i > 0) {
				if (tokens[i + 1][0] == tok_type::Number && tokens[i - 1][0] == tok_type::Number) {
					switch (tokens[i][0])
					{
					case tok_type::Mul: 
						tokens[i - 1][1] = tokens[i - 1][1] * tokens[i + 1][1]; 
						break;
					case tok_type::Div: 
						tokens[i - 1][1] = tokens[i - 1][1] / tokens[i + 1][1];
						break;
					case tok_type::Mod: 
						tokens[i - 1][1] = tokens[i - 1][1] % tokens[i + 1][1];
						break;
					}
					remove_row(tokens, tok_count, cols, i);
					remove_row(tokens, tok_count, cols, i);
					i = 0;
				}
				else {
					std::cerr << "Syntax error!" << std::endl;
					delete_tokens(tokens, tok_count);
					return nullptr;
				}
			}
			else {
				std::cerr << "Syntax error!" << std::endl;
				delete_tokens(tokens, tok_count);
				return nullptr;
			}
		}
	}
	return execute_adds_and_subs(tokens, tok_count);
}
long long** execute_pow(long long** tokens, std::size_t& tok_count) {
	std::size_t cols = 2;
	for (int i = 0; i < tok_count; i++)
	{
		if (tokens[i][0] == tok_type::Pow) {
			if (i > 0) {
				if (tokens[i + 1][0] == tok_type::Number && tokens[i - 1][0] == tok_type::Number) {
					tokens[i-1][1] = std::pow(tokens[i - 1][1], tokens[i + 1][1]);
					remove_row(tokens, tok_count, cols, i);
					remove_row(tokens, tok_count, cols, i);
					i = 0;
				}
				else {
					std::cerr << "Syntax error!" << std::endl;
					delete_tokens(tokens, tok_count);
					return nullptr;
				}
			}
			else {
				std::cerr << "Syntax error!" << std::endl;
				delete_tokens(tokens, tok_count);
				return nullptr;
			}
		}
	}
	return execute_factor(tokens, tok_count);
}
long long** execute_unary_minus(long long** tokens, std::size_t& tok_count)
{
	std::size_t cols = 2;
	for (int i = tok_count-1; i >= 0; i--)
	{
		if (tokens[i][0] == tok_type::Minus) {
			if (i > 0) {
				if (tokens[i + 1][0] == tok_type::Number && tokens[i - 1][0] != tok_type::Number)
				{
					remove_row(tokens, tok_count, cols, i);
					tokens[i][1] = -tokens[i][1];
				}
			}
			else {
				remove_row(tokens, tok_count, cols, i);
				tokens[i][1] = -tokens[i][1];
			}
		}
	}
	return execute_pow(tokens, tok_count);
}
long long ** execute(long long ** tokens, std::size_t& tok_counts)
{
	return execute_unary_minus(tokens, tok_counts);
}

#endif
