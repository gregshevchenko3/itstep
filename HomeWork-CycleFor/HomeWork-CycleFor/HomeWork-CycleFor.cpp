#include <iostream>

#include <windows.h> // ��� �� ��� �� �������� ������� �������� ����, ��� �����, �� � ���������� ���������� ����������

// 1. ��������� ���� for �������� ���������, ������� �������� ����� � �������. 
// ��������� ������� � ���������� �������  �������� � ����������;
double Pow(double a, int b) {
	double result = 1.;
	for (auto it = 0; it < b; it++)
		result *= a;
	return result;
}

// 2. ��������� ���� for ������� �� ����� ������� ASCII - �������� �� 16 �������� � ������;
void ASII_table() {
	for (unsigned char ch = 0; ch < 0xFF; ch++) {
		if (!(ch & 0xF)) std::cout << std::endl << "\t";
		std::cout << ch << " ";
	}
}

// 3. ��������� ��������� for ������� �� ����� ������� ���������;
void Multiplication_table() {
	for (auto b = 1; b <= 10; b++) {
		std::cout << "\t";
		for (auto a = 2; a <= 5; a++)
			std::cout << a << " * " << b << " = " << a * b << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto b = 1; b <= 10; b++) {
		std::cout << "\t";
		for (auto a = 6; a <= 9; a++)
			std::cout << a << " * " << b << " = " << a * b << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//4. ��������� ��������� for ������� �� ����� ������� ��������;
void pythagoras() {
	for (auto r = -1; r <= 10; r++) {
		std::cout << "\t";
		std::cout.width(4);
		switch (r) {
		case -1:
			std::cout << " " << "|";
			break;
		case 0:
			std::cout << "----" << "+";
			break;
		default:
			std::cout << r << "|";
		}
		for (auto c = 1; c <= 10; c++) {
			std::cout.width(4);
			switch (r) {
			case -1:
				std::cout << c;
				break;
			case 0:
				std::cout << ("----");
				break;
			default:
				std::cout << r * c;
			}
		}
		std::cout << std::endl;
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ukr");

	double a;
	int b;
	std::cout << "1. ���������� �������." << std::endl << "double a> ";
	std::cin >> a;
	std::cout << "int b> ";
	std::cin >> b;
	std::cout << "a^b = " << Pow(a, b) << std::endl << std::endl;

	std::cout << "2. ������� ASCII." << std::endl;

	SetConsoleCP(866);
	SetConsoleOutputCP(866);

	ASII_table();

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << std::endl << std::endl;

	std::cout << "3. ������� ��������." << std::endl;
	Multiplication_table();
	std::cout << std::endl;

	std::cout << "4. ������� ϳ������." << std::endl;
	pythagoras();
	std::cout << std::endl;
}
