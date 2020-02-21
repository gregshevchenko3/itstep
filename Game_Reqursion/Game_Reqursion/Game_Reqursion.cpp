#include <clocale>
#include <algorithm>
#include <conio.h>

// Ігрове поле
char field[3][3];

// вумна консоль.
#include "console.h"
#include "menu.h"
#include "status_bar.h"
#include "game.h"

void init_field() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			field[i][j] = ' ';
		}
	}
};


/**
 * Рахує рейтинг по рядках, стовцях, та дiагонялях. 
 * Результат записує в масив, що переданий в якостi аргументу.
 * 
 * Виикористовується при перевiрцi iгрового поля та при прорахунку ходу ком'пютером.
 */
void create_rating_vector(int atoms[8]) {
	for (auto i = 0; i < 8; i++) atoms[i] = 0;
	for (auto i = 0; i < 3; i++) {
		for (auto j = 0; j < 3; j++) {
			// сума рядка
			if (field[i][j] == 'X') atoms[i] += 1;
			if (field[i][j] == 'O') atoms[i] -= 1;
			// сума колонки
			if (field[j][i] == 'X') atoms[i + 3] += 1;
			if (field[j][i] == 'O') atoms[i + 3] -= 1;
		}
		// сума по основнiй дiагоналi
		if (field[i][i] == 'X') atoms[6] += 1;
		if (field[i][i] == 'O') atoms[6] -= 1;
		// сума по побiчнiй дiагоналi
		if (field[i][2 - i] == 'X') atoms[7] += 1;
		if (field[i][2 - i] == 'O') atoms[7] -= 1;
	}
}
/**
 * Вирiщує куди вписувать "O". результат передає в змiннi, переданi за посиланнями res_row, res_col;
 * koef_idx - фактор, за яким приймається рiшення, це позицiя елемента в масивi рейтигiв, обчисленому
 * функцiєю create_rating_vector().
 */
void solve(int koef_idx, int& res_row, int& res_col) {
	if (koef_idx < 3) {
		res_row = koef_idx;
		res_col = std::find(field[koef_idx], field[koef_idx] + 3, ' ') - field[koef_idx];
	}
	else if (koef_idx < 6) {
		res_col = koef_idx % 3;
		char ar[3] = { field[0][res_col], field[1][res_col], field[2][res_col] };
		res_row = std::find(ar, ar + 3, ' ') - ar;
	}
	else if (koef_idx == 6) {
		char ar[3] = { field[0][0], field[1][1], field[2][2] };
		int i = std::find(ar, ar + 3, ' ') - ar;
		res_col = res_row = i;
	}
	else {
		char ar[3] = { field[0][2], field[1][1], field[2][0] };
		int i = std::find(ar, ar + 3, ' ') - ar;
		res_row = i, res_col = 2 - i;
	}
}
/**
 * Власне логiка компа. Без передбачень. Можливостi навчання i т.д.
 * Прото тупий кiнцевий автомат. Деталi по тексту функцiї
 */
void calc_comp_move() {
	int atoms[8];
	create_rating_vector(atoms);

	int row = 0, col = 0;
	auto mx = std::max_element(atoms, atoms + 8), mn = std::min_element(atoms, atoms + 8);
	int mx_idx = mx - atoms, mn_idx = mn - atoms;
	/*
	 * Якщо сума елементiв в ряду, в стовпцi, або в дiагональ матрицi дорiвнює -2, то а цим ходом можна виграти,
	 * вписавши у вiльну клiтинку ряду, стовпця, дiагоналi вiдповiдно "O"
	 */
	if (*mn == -2)
		solve(mn_idx, row, col);
	else
		/*
		 * Якщо сума елементiв в ряду, в стовпцi, або в матрицi дорiвнює 2, то аби не програти - вписуємо
		 * у вiльну клiтинку ряду, стовпця, дiагоналi вiдповiдно "O"
		 */
		if (*mx == 2)
			solve(mx_idx, row, col);
		else
			/*
			 * Якщо нi виграш, нi програш не грозять, записуємо "O" у вiльну клiтинку ряду, стовпця, дiагоналi вiдповiдно, якщо сума елементiв - найменша.
			 */
			solve(mn_idx, row, col);
	field[row][col] = 'O';
}
// Що натиснув гравець?
char* get_field() {
	int key;
	do {
		key = _getch();
		if (key == 27) {
			return 0;
		};
	} while (key < 49 || key > 57);
	int row = 2 - ((key - 49) / 3), col = (key - 49) % 3;
	return &field[row][col];
}


void check(bool player, int move_count) {
	int atoms[8];
	create_rating_vector(atoms);

	if (3 == *std::max_element(atoms, atoms+8)) {
		int pos = std::find(atoms, atoms + 8, 3) - atoms;
		update(player, move_count, pos);
		return;
	}
	if (-3 == *std::min_element(atoms, atoms+8)) {
		int pos = std::find(atoms, atoms + 8, -3) - atoms;
		update(player, move_count, pos);
		return;
	}
	
	if (move_count == 0){
		update(player, move_count, -1);
		return;
	}
	// гравець натиснув Esc (Здався)
	if (move_count == -1) {
		if (player) {
			update(player, move_count, -1);
			return;
		}
	}
	move(player, move_count);
}
/**
 * Хiд.
 * player == true - грає живий гравець
 * player == false - грає комп'ютер.
 *
 * move_count > 0 - гра продовжується. Через цей параметр вiдслiдковується нiчия, та технiчна поразка (гравець здався)
 */
void move(bool player, int move_cont) {
	if (player) {
		char* fld = 0;
		do {
			fld = get_field();
			if (!fld) {
				check(player, -1);
				return;
			}
		} while (*fld != ' ');
		if (*fld != 'X' && *fld != 'O') {
			if (player) *fld = 'X';
			else
				*fld = 'O';
		}
	}
	else {
		calc_comp_move();
	}
	update(!player, move_cont - 1, -1);
}
/**
 * Малюює поле.
 * player == true - грає живий гравець
 * player == false - грає комп'ютер.
 *
 * move_count > 0 - гра продовжується. Через цей параметр вiдслiдковується нiчия, та технiчна поразка (гравець здався)
 */
void update(bool player, int move_count, int pos) {
	console_ostream<char, std::char_traits<char>> console(std::cout.rdbuf());
	coord2d size = console.get_console_size();
	short y_offset = (size.y - 5) / 2, x_offset = (size.x - 13) / 2;
	// 13 символiв на 5 строк
	
	system("cls");
	console << attr(10);
	for (int i = 0; i < 3; i++) {
		console << coord2d(x_offset, y_offset++);
		if (i == 0)
			console << "+---+---+---+" << std::endl;
			console << coord2d(x_offset, y_offset++);
		for (int j = 0; j < 3; j++) {
			char ch = field[i][j];
			switch (pos) {
			case 0:
			case 1:
			case 2:
				if (i == pos) {
					console << "|" << attr( 0 | (player)? BACKGROUND_RED:BACKGROUND_GREEN ) << " " << ch << " " << attr(10);
				} else 
					console << "| " << ch << attr(10) << " ";
				break;
			case 3:
			case 4:
			case 5:
				if (j == pos % 3) {
					console << "|" << attr(0 | (player) ? BACKGROUND_RED : BACKGROUND_GREEN) << " " << ch << " " << attr(10);
				}
				else 
					console << "| " << ch << attr(10) << " ";
				break;
			case 6:
				if (i == j) {
					console << "|" << attr(0 | (player) ? BACKGROUND_RED : BACKGROUND_GREEN) << " " << ch << " " << attr(10);
				}
				else
					console << "| " << ch << attr(10) << " ";
				break;
			case 7:
				if (i == 2 - j) {
					console << "|" << attr(0 | (player) ? BACKGROUND_RED : BACKGROUND_GREEN) << " " << ch << " " << attr(10);
				}
				else console << "| " << ch << attr(10) << " ";
				break;
			default:
				console << "| " << ch << attr(10) << " ";
			}
		}
		console << attr(10) <<  "|" << std::endl;
		console << coord2d(x_offset, y_offset) << "+---+---+---+" << std::endl;
	}
	console << std::endl;
	console << coord2d(x_offset, y_offset+1);
	if (pos >= 0) {
		if (!player)
			console << "Ви виграли!!!";
		else 
			console <<  "Ви програли!!!" ;
	}
	if (move_count == -1) console << "Ви здалися!!!";
	if (move_count == 0) console << "Нiчия";

	if (pos != -1 || move_count == -1 || move_count == 0) {
		show_status_bar(" Натиснiть будь-яку клавiшу ");
		getch();
		return;
	}
	if (player)
		console << "Ваш хiд";
	show_status_bar(" 1-9 вибiр поля, Esc - здатися ");
	check(player, move_count);
}

int main()
{
	menu_item items[] = {
		{menu_item::mtype::button, false, true, "Нова гра", New_game },
		{menu_item::mtype::button, false, false, "Вихiд", Exit},
	};
	
	auto console = GetStdHandle(STD_OUTPUT_HANDLE);
	auto size = GetLargestConsoleWindowSize(console);
	
	size.X = 40;
	size.Y = 20;
	SMALL_RECT windowSize = { 0, 0, 39, 19 };

	SetConsoleWindowInfo(console, true, &windowSize);
	SetConsoleScreenBufferSize(console, size);
	std::setlocale(LC_ALL, "en_US.utf8");
	menu_navigation(items, 2);

	return 0;
}