#pragma once
#include <iostream>
#include <windows.h>

struct coord2d {
	unsigned short x, y;
	coord2d(unsigned short _x, unsigned short _y) : x(_x), y(_y) {};
	coord2d(const coord2d& other) {
		x = other.x;
		y = other.y;
	}
	coord2d& operator=(const coord2d& other) {
		x = other.x;
		y = other.y;
		return *this;
	}
};

struct attr {
	short m_attr;
	attr(short attr) : m_attr(attr) {};
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& i, attr a) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a.m_attr);
	return i;
};

template <class _Elem, class _Traits>
class console_ostream : public std::basic_ostream<_Elem, _Traits> {
public:
	console_ostream(std::basic_streambuf<_Elem, _Traits>* sb)
		: std::basic_ostream<_Elem, _Traits>(sb) {};

	coord2d get_console_size() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		int cols, rows;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		return coord2d(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
	}

	coord2d get_current_cursor_position() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		return coord2d(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}

	

};

template <class _Elem, class _Traits>
console_ostream<_Elem, _Traits>& operator<<(console_ostream<_Elem, _Traits>& i, coord2d new_position) {
	COORD coord;
	coord.X = new_position.x;
	coord.Y = new_position.y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return i;
};