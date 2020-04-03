#include <clocale>
#include <windows.h>

#include "table_config.h"
#include "Common/game.h"
#include "Common/keypad.h"

int main() {
	std::setlocale(LC_ALL, "en_US.utf8");
	bool start;
	int score, best;

	text_graphics(true);
	do {
		int content[ROWS][COLS];
		if (generate(content, score, best)) 
		{
		}
		else 
			break;
	} while (start);
	text_graphics(false);
}