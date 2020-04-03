#include <clocale>
#include <ctime>

#include "Common/keypad.h"
#include "table_config.h"
#include "Common/game.h"



void fills_with_unique_random_values(int content[ROWS][COLS], int min, int max);
bool find(int num, int in[ROWS][COLS]);


void main() {
	std::setlocale(LC_ALL, "en_US.utf8");
	srand(time(0));
	int content[4][4] = { {0} };
	std::string filename = "15 Puzzle Game.txt";

	bool start;
	int score = 0, best = game_score_deserialize(filename);
	text_graphics(true);
	do {
		std::system("cls");
		int content[ROWS][COLS] = { {} };
		if (generate(content, score, best)) {
			std::stringstream ss;
			ss << u8"Гру завершено! Ваш результат - " << score << "\n" << u8"Бажаєте зiграти ще?";
			if (best != 0) {
				if (score < best) {
					best = score;
				}
			}
			else {
				best = score;
			}
			score = 0;
			std::system("cls");
			start = drawYesNoMsg(ss.str());
		}
		else
			break;
	} while (start);
	game_score_serialize(filename, best);
	text_graphics(false);
}

void fills_with_unique_random_values(int content[ROWS][COLS], int min, int max) {
	for(auto i = 0; i < ROWS; i++)
	{
		for (auto j = 0; j < COLS; j++) 
		{
			if (i == (ROWS - 1) && j == (COLS - 1)) break;
			bool found = false;
			int num;
			do {
				num = rand() % (max - min + 1) + min;
				found = find(num, content);
			} while (found);
			content[i][j] = num;
		}
	}
}
bool find(int num, int in[ROWS][COLS])
{
	for (auto i = 0; i < ROWS; i++)
	{
		for (auto j = 0; j < COLS; j++)
		{
			if (in[i][j] == num) return true;
		}
	}
	return false;
}
bool generate(int content[ROWS][COLS], int& score, int& best, bool entry) {
	system("cls");
	fills_with_unique_random_values(content, 1, 15);
	return draw(content, score, best);
}
bool move(int content[ROWS][COLS], int& score, int& best) {
	static int cursor_row = 3, cursor_col = 3;
	int prev_cursor_row = cursor_row, prev_cursor_col = cursor_col;
	if (check(content)) {
		cursor_row = cursor_col = 3;
		return true;
	}
	while (1) {
		switch (get_way()) {
		case enWay::up:
			if (cursor_row)
				cursor_row--;
			else
				continue;
			break;
		case enWay::down:
			if (cursor_row < 3) cursor_row++;
			else
				continue;
			break;
		case enWay::left:
			if (cursor_col)
				cursor_col--;
			else
				continue;
			break;
		case enWay::right:
			if (cursor_col < 3) cursor_col++;
			else
				continue;
			break;
		case enWay::Exit:
			std::system("cls");
			if (drawYesNoMsg(u8"Ви вже йдете?"))
				return false;
			std::system("cls");
		}
		break;
	}

	int a = content[cursor_row][cursor_col];
	content[cursor_row][cursor_col] = content[prev_cursor_row][prev_cursor_col];
	content[prev_cursor_row][prev_cursor_col] = a;
	score++;
	return draw(content, score, best);
}
bool check(int content[ROWS][COLS]) {
	bool result = true;
	for(auto i = 0; i < ROWS; i++)
	{ 
		for (auto j = 0; j < COLS; j++)
		{
			if (i == (ROWS - 1) && j == (COLS - 1)) break;
			result = result && ((ROWS*i + j + 1) == content[i][j]);
		}
	}
	return result;
}