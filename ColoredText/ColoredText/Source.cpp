#include "console.h"



int main() {
	console_ostream<char, std::char_traits<char>> con(std::cout.rdbuf());
	coord2d size = con.get_console_size();
	coord2d cursor = con.get_current_cursor_position();



	con << "Console width: " << size.x << std::endl;
	cursor = con.get_current_cursor_position();
	con << coord2d(cursor.x + 20, cursor.y) << attr(FOREGROUND_RED)
		<< "Console height: " << size.y << std::endl << attr(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
		<< "Console cursor:\t" << cursor.x << ", " << cursor.y << std::endl;

}