#pragma once

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/exception/all.hpp>

using at_char = boost::error_info<struct at_char_tag, char*>;

inline void ParserError(const char *what, char* where, const char* function, const char* filename, const int line)
{
	boost::throw_exception(
		::boost::enable_error_info(std::runtime_error(what))
		<< at_char(where)
#ifdef _DEBUG
		<< ::boost::throw_function(function)
		<< ::boost::throw_file(filename)
		<< ::boost::throw_line(line)
#endif;
	);
}

inline void MathError(const char *what, const char* function, const char* filename, const int line) {
	boost::throw_exception(
		::boost::enable_error_info(std::invalid_argument(what))
#ifdef _DEBUG
		<< ::boost::throw_function(function)
		<< ::boost::throw_file(filename)
		<< ::boost::throw_line(line)
#endif
	);
}

inline void CatchError(const std::exception& e, const char* whole_expression) {
	std::cerr << "Error: " <<  e.what() << std::endl << std::endl;
	if (const char* const *extra = boost::get_error_info<at_char>(e)) {
		// Замість тернарного оператора, потрібно придумати як тикнути носом користувача в помилку.
		char* tmp_ = (char*)whole_expression;
		
		std::string error_underline = "";
		while (tmp_ < *extra-1) {
			error_underline += (*tmp_++ != '\t') ? " " : "\t";
		}
		error_underline += "~~~";
		std::cout << "\t" << ((error_underline[0] == '^')?" ":"") << whole_expression << std::endl;
		std::cout << "\t" << error_underline << std::endl << std::endl;
		error_underline.~basic_string();
	}
#ifdef _DEBUG
	if (const char* const *extra = boost::get_error_info<::boost::throw_function>(e)) {
		std::cout << "\tFUNCTION : " << *extra << std::endl;
	}
	if (const char* const *extra = boost::get_error_info<::boost::throw_file>(e)) {
		std::cout << "\tFILE :" << *extra << std::endl;
	}
	if (const int* extra = boost::get_error_info<::boost::throw_line>(e)) {
		std::cout << "\tLINE :" << *extra << std::endl;
	}
#endif
}
