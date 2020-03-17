#include <iostream>
#include <string>
#include <clocale>
#include <ctime>

#include <windows.h>

const size_t num_puzzels = 16;
enum keypad {
	up = 72, down = 80, left = 75, right = 77
};
int scene[num_puzzels] = { 0 };

/** 
 * В Function-HomeWork я трохи перемудрив з подібною  функцією, і написав таке, що не може бути застосовано... ніде.
 * це спроба №2. 
 *
 * Заповнення масиву випдковими значенням, що знаходяться в інтервалі від min до max, і ні разу не зустрічалися в цьому масиві. 
 */
void fills_with_unique_random_values(int array[], size_t len, int min, int max);
/** Кількість символів в строці utf-8 */
size_t u8length(const char* u8str);

/** Ініціалізація*/
void init_scene();
/** Перерисовка поля*/
void redraw_scene();
/** Лічильник ходів */
int move_count();
/** Хід */
void move();
/** Перевірка ігрового поля*/
void check();
/** */
void print_move_count(int cnt);

inline void get_console_size(short& width, short& height) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void main() {
	std::setlocale(LC_ALL, "en_US.utf8");
	srand(time(0));
	init_scene();
}

void check() {
	bool result = true;
	for (auto i = 0; i < num_puzzels - 1; i++) {
		result = result && (scene[i] == i + 1);
	}
	if (result) {
		print_move_count(move_count());
		return;
	}
	else {
		move();
	}
}
void redraw_scene() {
	static short width = 0, height = 0, start_x, start_y;
	static bool once = true;
	
	const char *top_border = u8"\u2554\u2550\u2550\u2564\u2550\u2550\u2564\u2550\u2550\u2564\u2550\u2550\u2557",
		*bottom_border = u8"\u255A\u2550\u2550\u2567\u2550\u2550\u2567\u2550\u2550\u2567\u2550\u2550\u255d",
		*internal_border = u8"\u255f\u2500\u2500\u253c\u2500\u2500\u253c\u2500\u2500\u253c\u2500\u2500\u2562",
		*left_right_row_border = u8"\u2551",
		*left_right_cell_border = u8"\u2502",
		*cursor = u8"\u2592\u2592";

	static std::string help_line = u8"\u2194 \u2195 - Перемiщення пустої клiтинки Esc - Вихiд";
	if (once) {
		get_console_size(width, height);
		start_x = (width - u8length(top_border)) / 2;
		start_y = (height - 2 * sqrt(num_puzzels) - 1) / 2;
		for (auto i = u8length(help_line.c_str()); i < width - 5; i++) {
			help_line += u8" ";
		};
		once = false;
	}

	COORD coord;
	coord.X = start_x;
	coord.Y = start_y;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << top_border;
	coord.Y++;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for (auto index = 0; index < num_puzzels; index++) {
		if (index % 4 == 0) {
			if (index / 4 >= 1) {
				coord.Y++;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				std::cout << internal_border;
				coord.Y++;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			}
			std::cout << left_right_row_border;
		}
		auto v = *(scene + index);
		if (v != 0) {
			std::cout.width(2);
			std::cout << v;
		}
		else {
			std::cout << cursor;
		}
			
		if (index % 4 == 3)
			std::cout << left_right_row_border;
		else
			std::cout << left_right_cell_border;
	}
	coord.Y++;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << bottom_border;

	coord.X = 0;
	coord.Y =  height - 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF << 4);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << help_line;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	check();
}

#include <sstream>

void print_move_count(int cnt){
	static short width = 0, height = 0, start_y;
	static bool once = true;

	if (once) {
		get_console_size(width, height);
		start_y = (2 + height + 2 * sqrt(num_puzzels) - 1) / 2;
		once = false;
	}
	
	const char* msg2 = u8"Хочете зiграти ще раз? (y - так/n - нi) ";

	std::stringstream ss;
	ss << u8"Ви завершили гру за " << cnt << u8" ходiв";
	std::string msg = ss.str();

	COORD coord;
	// Стерти строку справки
	coord.X = 0;
	coord.Y = height - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for (int i = 0; i < width; i++) std::cout << " ";

	coord.Y = start_y;
	coord.X = (width - u8length(msg.c_str())) / 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << msg;
		
	coord.Y = start_y + 1;
	coord.X = (width - u8length(msg2)) / 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << msg2;

	char ch;
	std::cin >> ch;
	if (ch == 'y')
		init_scene();
	if (ch == 'n') return;
}

int move_count() {
	static int m_cnt = 0;

	int r = m_cnt++;
	return r;
};

void fills_with_unique_random_values(int array[], size_t len, int min, int max) {
	for (auto i = 0; i < len; i++) {
		int *it;
		do {
			array[i] = std::rand() % (max - min + 1) + min;
			it = std::find(array, array + i, array[i]);
		} while (it != array + i);
	}
};
size_t u8length(const char* u8str) {
	const char u8ch_begin = ((1 << 7) | (1 << 6));
	size_t result = 0;
	const char* begin = u8str;
	while (*begin) {
		if (*begin & (1 << 7)) {
			if (*begin & u8ch_begin == u8ch_begin) {
				// Початок багатобайтового символу - як мініму два старші біти будуть встановлені
				result++;
			}
		}
		else {
			// однобайтовий символ
			result++;
		}
		begin++;
	}
	return result;
};

void init_scene() {
	system("cls");
	fills_with_unique_random_values(scene, 15, 1, 15);
	redraw_scene();
}

#include <conio.h>
void move() {
	static int cursor_row = 3, cursor_col = 3;

	int current_index = cursor_row * 4 + cursor_col;

	while (1) {
		int ch = _getch();
		if (224 == ch) {
			int ch = _getch();
			switch (ch) {
			case up:
				if (cursor_row)
					cursor_row--;
				else
					continue;
					break;
			case down:
				if (cursor_row < 3) cursor_row++;
				else
					continue;
				break;
			case left:
				if (cursor_col)
					cursor_col--;
				else
					continue;
				break;
			case right:
				if (cursor_col < 3) cursor_col++;
				else
					continue;
				break;
			default:
				continue;
			}
		}
		else {
			switch (ch) {
			case 27: return;
			}
		}
		break;
	}

	auto tmp = *(scene + cursor_row * 4 + cursor_col);
	*(scene + cursor_row * 4 + cursor_col) = *(scene + current_index);
	*(scene + current_index) = tmp;

	move_count();
	redraw_scene();
}
