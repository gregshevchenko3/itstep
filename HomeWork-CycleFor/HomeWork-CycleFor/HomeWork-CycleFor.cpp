#include <iostream>

#include <windows.h> // ћен≥ ще дос≥ не хочетьс€ ставити рос≥йську мову, тим б≥льше, що € користуюс€ здеб≥льшого украњнською

// 1. »спользу€ цикл for написать программу, котора€ возводит число в степень. 
// ќснование степени и показатель степени  ввод€тс€ с клавиатуры;
double Pow(double a, int b) {
	double result = 1.;
	for (auto it = 0; it < b; it++)
		result *= a;
	return result;
}

// 2. »спользу€ цикл for вывести на экран таблицу ASCII - символов по 16 символов в строке;
void ASII_table() {
	for (unsigned char ch = 0; ch < 0xFF; ch++) {
		if (!(ch & 0xF)) std::cout << std::endl << "\t";
		std::cout << ch << " ";
	}
}

// 3. »спользу€ вложенный for вывести на экран таблицу умножени€;
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

//4. »спользу€ вложенный for вывести на экран таблицу ѕифагора;
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
	std::cout << "1. ќбчисленн€ степен€." << std::endl << "double a> ";
	std::cin >> a;
	std::cout << "int b> ";
	std::cin >> b;
	std::cout << "a^b = " << Pow(a, b) << std::endl << std::endl;

	std::cout << "2. “аблиц€ ASCII." << std::endl;

	SetConsoleCP(866);
	SetConsoleOutputCP(866);

	ASII_table();

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	std::cout << std::endl << std::endl;

	std::cout << "3. “аблиц€ множенн€." << std::endl;
	Multiplication_table();
	std::cout << std::endl;

	std::cout << "4. “аблиц€ ѕ≥фагора." << std::endl;
	pythagoras();
	std::cout << std::endl;
}
