#include <iostream>
#include <conio.h>

char field[3][3] = {
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' }
};

bool equ_vert(int vert_num, char c) {
	bool result = true;
	for (auto i = 0; i < 3; i++)
		result = result && (field[i][vert_num] == c);
	return result;
}
bool equ_horiz(int horiz_num, char c) {
	bool result = true;
	for (auto i = 0; i < 3; i++)
		result = result && (field[horiz_num][i] == c);
	return result;
}
bool equ_diag(char ch) {
	bool result = true, res2 = true;
	for (auto r = 0, c = 0; c < 3 && r < 3; c++, r++)
		result = result && (field[r][c] == ch);

	for (auto r = 0, c = 2; c >= 0 && r < 3; c--, r++)
		res2 = res2 && (field[r][c] == ch);

	return result || res2;
}

bool Player_Win(char ch) {
	bool result = false;
	for (int i = 0; i < 3; i++) {
		result = result || equ_vert(i, ch) || equ_horiz(i, ch);
	}
	return result || equ_diag(ch);
}

char* get_field(int NumLockKey) {
	switch (NumLockKey) {
	case 49: return &field[2][0];
	case 50: return &field[2][1];
	case 51: return &field[2][2];
	case 52: return &field[1][0];
	case 53: return &field[1][1];
	case 54: return &field[1][2];
	case 55: return &field[0][0];
	case 56: return &field[0][1];
	case 57: return &field[0][2];
	default:
		return 0;
	}
}

void check(bool, int);
void move(bool, int);

void update(bool start, int move_count) {
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
	if (start)
		std::cout << "Хід гравця №1:" << std::endl;
	else
		std::cout << "Хід гравця №2:" << std::endl;
	check(start, move_count);
}

void move(bool player, int move_cont) {
	int key;
	do {
		key = _getch();
		if (key == 27) {
			check(player, -1);
		};
	}
	while (key < 49 || key > 57);

	char* fld = get_field(key);
	
	if (*fld != 'X' && *fld != 'O') {
		if (player) *fld = 'X';
		else
			*fld = 'O';
	}
	update(!player, move_cont - 1);
	
}

void check(bool start, int move_count) {
	if (Player_Win('X')) {
		std::cout << "Гравець №1 - виграв!!!" << std::endl;
		return;
	}
	if (Player_Win('O')) {
		std::cout << "Гравець №2 - виграв!!!" << std::endl;
		return;
	}
	if (move_count == 0) {
		std::cout << "Нічия..." << std::endl;
		return;
	}
	if (move_count == -1) {
		if (start) {
			std::cout << "Гравець №1 - здався!!!" << std::endl;
			std::cout << "Гравець №2 - виграв!!!" << std::endl;
		}
		else {
			std::cout << "Гравець №2 - здався!!!" << std::endl;
			std::cout << "Гравець №1 - виграв!!!" << std::endl;
		}
	}
	move(start, move_count);
}
#include "windows.h"
int main()
{
	setlocale(LC_ALL, "Ukranian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	update(true, 9);
	std::cout << "GAME OVER!" << std::endl;
	return 0;
}