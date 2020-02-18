#include <iostream>
#include <algorithm>
#include <conio.h>

// Ігрове поле
char field[3][3] = { 
	{' ', ' ', ' ',},
	{' ', ' ', ' ',},
	{' ', ' ', ' ',},
};

// Комп'ютерний гравець.
void calc_comp_move();
// Перевірка ігрового поля;
void check(bool, int);
// Хід 
void move(bool, int);
// Отрисовка поля
void update(bool player, int move_count);
/**
 * Рахує рейтинг по рядках, стовцях, та діагонялях. 
 * Результат записує в масив, що переданий в якості аргументу.
 * 
 * Виикористовується при перевірці ігрового поля та при прорахунку ходу ком'пютером.
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
		// сума по основній діагоналі
		if (field[i][i] == 'X') atoms[6] += 1;
		if (field[i][i] == 'O') atoms[6] -= 1;
		// сума по побічній діагоналі
		if (field[i][2 - i] == 'X') atoms[7] += 1;
		if (field[i][2 - i] == 'O') atoms[7] -= 1;
	}
}
/**
 * Виріщує куди вписувать "O". результат передає в змінні, передані за посиланнями res_row, res_col;
 * koef_idx - фактор, за яким приймається рішення, це позиція елемента в масиві рейтигів, обчисленому
 * функцією create_rating_vector().
 */
void solve(int koef_idx, int& res_row, int& res_col) {
	if (koef_idx < 3) {
		res_row = koef_idx;
		res_col = std::find(field[koef_idx], field[koef_idx] + 3, ' ') - field[koef_idx];
	}
	else if (koef_idx < 6) {
		res_col = koef_idx;
		char ar[3] = { field[0][koef_idx], field[1][koef_idx], field[2][koef_idx] };
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
 * Власне логіка компа. Без передбачень. Можливості навчання і т.д.
 * Прото тупий кінцевий автомат. Деталі по тексту функції
 */
void calc_comp_move() {
	int atoms[8];
	create_rating_vector(atoms);

	int row = 0, col = 0;
	auto mx = std::max_element(atoms, atoms + 8), mn = std::min_element(atoms, atoms + 8);
	int mx_idx = mx - atoms, mn_idx = mn - atoms;
	/*
	 * Якщо сума елементів в ряду, в стовпці, або в діагональ матриці дорівнює -2, то а цим ходом можна виграти,
	 * вписавши у вільну клітинку ряду, стовпця, діагоналі відповідно "O"
	 */
	if (*mn == -2)
		solve(mn_idx, row, col);
	else
		/*
		 * Якщо сума елементів в ряду, в стовпці, або в матриці дорівнює 2, то аби не програти - вписуємо
		 * у вільну клітинку ряду, стовпця, діагоналі відповідно "O"
		 */
		if (*mx == 2)
			solve(mx_idx, row, col);
		else
			/*
			 * Якщо ні виграш, ні програш не грозять, записуємо "O" у вільну клітинку ряду, стовпця, діагоналі відповідно, якщо сума елементів - найменша.
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
		std::cout << "Ви виграли!!!" << std::endl;
		return;
	}
	if (-3 == *std::min_element(atoms, atoms+8)) {
		std::cout << "Ви програли!!!" << std::endl;
		return;
	}
	
	if (move_count == 0){
		std::cout << "Нічия..." << std::endl;
		return;
	}
	// гравець натиснув Esc (Здався)
	if (move_count == -1) {
		if (player) {
			std::cout << "Ви здалися!!!" << std::endl;
			return;
		}
	}
	move(player, move_count);
}
/**
 * Хід.
 * player == true - грає живий гравець
 * player == false - грає комп'ютер.
 *
 * move_count > 0 - гра продовжується. Через цей параметр відслідковується нічия, та технічна поразка (гравець здався)
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
	update(!player, move_cont - 1);
}
/**
 * Малюює поле.
 * player == true - грає живий гравець
 * player == false - грає комп'ютер.
 *
 * move_count > 0 - гра продовжується. Через цей параметр відслідковується нічия, та технічна поразка (гравець здався)
 */
void update(bool player, int move_count) {
	system("cls");
	for (int i = 0; i < 3; i++) {
		if (i == 0)
			std::cout << "+---+---+---+" << std::endl;
		for (int j = 0; j < 3; j++) 
			std::cout << "| " << field[i][j] << " ";
		std::cout << "|" << std::endl;
		std::cout << "+---+---+---+" << std::endl;
	}
	std::cout << std::endl;
	if (player)
		std::cout << "Ваш хід" << std::endl;
	
	check(player, move_count);
}

#include "windows.h"
int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	setlocale(LC_ALL, "Ukranian");

	update(true, 9);
	std::cout << "GAME OVER!" << std::endl;
	return 0;
}