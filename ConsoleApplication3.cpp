// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

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
	}
}

void update() {
	system("cls");

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) std::cout << field[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	update();
	/*int fields = 9;
	bool player = true;
	while (fields > 0)
	{
		if (player) std::cout << "Player 1:";
		else
			std::cout << "Player 2:";

		int key = getch();
		if (key == 27) break;

		char* fld = get_field(key);
		if (*fld != 'X' && *fld != 'O') {
			if (player) *fld = 'X';
			else
				*fld = 'O';
			fields--;
		}

		update();

		if (player && Player_Win('X')) {
			std::cout << "Player 1: Win!!!" << std::endl;
			break;
		}
		if (!player && Player_Win('O')) {
			std::cout << "Player 2: Win!!!" << std::endl;
			break;
		}
		player = !(player);
	}
	std::cout << "GAME OVER!" << std::endl;*/
    return 0;
}

