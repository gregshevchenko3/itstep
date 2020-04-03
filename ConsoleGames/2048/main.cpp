#include "Common/keypad.h"
#include <clocale>
#include <ctime>

#include <windows.h>
#include "table_config.h"
#include "Common/game.h"


int move_left(int content[ROWS][COLS], bool& changes);
int move_right(int content[ROWS][COLS], bool& changes);
void shift_left(int content[ROWS][COLS], int row, int start_col = 0);
void shift_right(int content[ROWS][COLS], int row, int start_col = COLS - 1);
void transpose(const int context[ROWS][COLS], int result[COLS][ROWS]);

void main() {
	std::setlocale(LC_ALL, "en_US.utf8");
	srand(time(0));
	std::string filename = "2048";

	bool start;
	int score = 0, best = game_score_deserialize(filename);
	text_graphics(true);
	do {
		std::system("cls");
		int content[ROWS][COLS] = { {} };
		if (generate(content, score, best, true)) {
			std::stringstream ss;
			ss << u8"Гру завершено! Ваш результат - " << score << "\n" << u8"Бажаєте зiграти ще?";
			if (score > best) {
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

bool check(int content[ROWS][COLS], int& score, int& best)
{
	bool game_over = true;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS - 1; j++) {
			game_over = game_over && (content[i][j] != content[i][j + 1]) && (content[i][j] != 0) && (content[i][j + 1] != 0);
			game_over = game_over && (content[j][i] != content[j + 1][i]);
		}
	}
	return game_over;
}

bool move(int content[ROWS][COLS], int& score, int& best)
{
	if (check(content, score, best)) return true;
	enWay way = get_way();
	bool changes = false;
	int trans_content[COLS][ROWS];
	switch (way) {
	case enWay::left:
		score += move_left(content, changes);
		break;
	case enWay::right:
		score += move_right(content, changes);
		break;
	case enWay::up:
		transpose(content, trans_content);
		score += move_left(trans_content, changes);
		transpose(trans_content, content);
		break;
	case enWay::down:
		transpose(content, trans_content);
		score += move_right(trans_content, changes);
		transpose(trans_content, content);
		break;
	case enWay::Exit:
		std::system("cls");
		if (drawYesNoMsg(u8"Ви вже йдете?"))
			return false;
		std::system("cls");
	}
	if (changes)
	{
		return generate(content, score, best);
	}
	else
		return draw(content, score, best);
}

bool generate(int content[ROWS][COLS], int& score, int& best, bool start)
{
	int free_fields_in_row[ROWS];
	bool non_free = true;
	for (auto i = 0; i < ROWS; i++)
	{
		free_fields_in_row[i] = 0;
		for (auto j = 0; j < COLS; j++)
			if (content[i][j] == 0) free_fields_in_row[i]++;
		non_free = non_free && (free_fields_in_row[i] == 0);
	}
	if (!non_free) {
		int row = 0, column = 0;
		row = rand() % ROWS;
		while (!free_fields_in_row[row])
		{
			row = rand() % ROWS;
		}

		column = rand() % COLS;
		while (content[row][column] != 0)
		{
			column = rand() % COLS;
		}

		content[row][column] = rand() % 2 + 1;
		content[row][column] *= 2;
		if (start)
		{
			start = false;
			return generate(content, score, best);
		}
	}
	return draw(content, score, best);
}
int move_left(int content[ROWS][COLS], bool& changes)
{
	int result = 0;
	for (auto r = 0; r < ROWS; r++)
	{
		// Щоб не двигать пусті рядки, краще раз перевірити і продовжити.
		bool is_empty = true;
		for (auto c = 0; c < COLS; c++)
		{
			is_empty = is_empty && (content[r][c] == 0);
		}
		if (is_empty) continue;

		for (auto c = 0; c < COLS; c++)
		{
			// Сдвиги елементів в не пустих рядках
			int n = COLS - c; // Максимальна кількість здвигів.
			while (content[r][c] == 0 && n-- > 0) {
				shift_left(content, r, c);
				changes = changes || content[r][c] != 0;
			}

			// Сума однакових елементів.
			if (c > 0 && content[r][c] == content[r][c - 1] && content[r][c] != 0) {
				content[r][c - 1] += content[r][c];
				result += content[r][c - 1];
				shift_left(content, r, c);
				if (content[r][c - 1] == content[r][c]) c++;
				changes = true;
			}
		}
	}
	return result;
}

int move_right(int content[ROWS][COLS], bool& changes)
{
	int result = 0;
	for (auto r = 0; r < ROWS; r++)
	{
		// Щоб не двигать пусті рядки, краще раз перевірити і продовжити.
		bool is_empty = true;
		for (auto c = COLS - 1; c >= 0; c--)
		{
			is_empty = is_empty && (content[r][c] == 0);
		}
		if (is_empty) continue;

		for (auto c = COLS - 1; c >= 0; c--)
		{
			// Сдвиги елементів в не пустих рядках
			int n = c; // Максимальна кількість здвигів. 
			while (content[r][c] == 0 && n-- > 0)
			{
				shift_right(content, r, c);
				changes = changes || content[r][c] != 0;
			}
			// Сума однакових елементів.
			if (c < COLS - 1 && content[r][c] == content[r][c + 1] && content[r][c] != 0) {
				content[r][c + 1] += content[r][c];
				result += content[r][c + 1];
				shift_right(content, r, c);
				changes = true;
				if (content[r][c] == content[r][c + 1]) c--;
			}
		}
	}
	return result;
}

void shift_left(int content[ROWS][COLS], int row, int start_col) {
	for (auto i = start_col; i < COLS - 1; i++) {
		content[row][i] = content[row][i + 1];
	}
	content[row][COLS - 1] = 0;
}
void shift_right(int content[ROWS][COLS], int row, int start_col) {
	for (auto i = start_col; i >= 0; i--) {
		content[row][i] = content[row][i - 1];
	}
	content[row][0] = 0;
}

void transpose(const int context[ROWS][COLS], int result[COLS][ROWS])
{
	for (auto i = 0; i < ROWS; i++)
	{
		for (auto j = 0; j < COLS; j++)
		{
			result[j][i] = context[i][j];
		}
	}
}
