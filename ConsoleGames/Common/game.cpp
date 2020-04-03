#include <fstream>

#if !defined(ROWS) && !defined(COLS) && !defined(FIELD_WIDTH) && !defined(FIELD_HEIGHT) && \
	!defined(SELCTED_BUTTON_ATTR) && !defined (BUTTON_ATTR) && !defined(HELP_BAR) && !defined (TEXT_ATTR)
#include "table_config.h"
#endif
#include "game.h"

bool draw(int content[ROWS][COLS], int& score, int& best)
{
	std::string help[] = { u8"\u2194, \u2195 = Хiд", u8"Esc = Вихiд" };

	CONSOLE_SCREEN_BUFFER_INFO info;
	COORD offset;
	auto handle = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(handle, &info);
	offset.X = (info.srWindow.Right - info.srWindow.Left) / 2 - COLS * (FIELD_WIDTH + 2) - COLS;
	offset.Y = (info.srWindow.Bottom - info.srWindow.Top - ROWS * FIELD_HEIGHT - ROWS) / 2;
	drawTable(content, offset, zero_field_is_empty);

	offset.X = (info.srWindow.Right - info.srWindow.Left) / 2 + 5;
	SetConsoleCursorPosition(handle, offset);
	drawScore(score, best, offset);
	drawHelpBar(help, 2);
	return move(content, score, best);
}
void game_score_serialize(std::string filename, int score)
{
	std::ofstream f(filename, std::ios_base::out | std::ios_base::trunc);
	f.clear();
	f << score;
	f.close();
}

int game_score_deserialize(std::string filename)
{
	int result = 0;
	std::ifstream f(filename, std::ios_base::in);
	if (f.good()) {
		f >> result;
	}
	f.close();
	return result;
}
