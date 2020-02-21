#include <clocale>
#include "status_bar.h"
#include "u8string.h"
#include "console.h"

void show_status_bar(std::string bar) {
	console_ostream<char, std::char_traits<char>> console(std::cout.rdbuf());
	auto size = console.get_console_size();

	auto len = u8length(bar);
	if (len > size.x - 1) return;
	auto after = std::string((size.x - len - 1), ' ');
	console << coord2d(0, size.y - 1) << attr(BACKGROUND_GREEN);
	console << bar << after;
	console << attr(0);
}