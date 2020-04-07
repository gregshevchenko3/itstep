// Pointer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <clocale>
#include <iostream>

//#define POINTERS_BASICS
//#define POINTERS_AND_ARRAYS

int sum(int count, int value...);
int main()
{
	std::setlocale(LC_ALL, "en_US.utf8");
#ifdef POINTERS_BASICS
	int a = 2; // «м≥нна
	int *pa = &a; // ¬каз≥вник 
	std::cout << a << std::endl;
	std::cout << &a << std::endl;
	std::cout << pa << std::endl;
	std::cout << *pa << std::endl;

	int* pb;
	int b = 3;
	pb = &b;
#endif
#ifdef POINTERS_AND_ARRAYS
	const int n = 5;
	short arr[n] = { 3, 5,  8, 13, 21 };
	std::cout << *arr << std::endl;
	for (auto i = 0; i < n; i++)
	{
		std::cout << *(arr + i) << "\t";
	}
	std::cout << std::endl;
	short *p_arr = arr;
	for (auto i = 0; i < n; i++)
	{
		std::cout << *++p_arr << "\t";
	}
	std::cout << std::endl;
#endif

	std::cout << sum(3, 1, 2, 3) << std::endl;

}

int sum(int count, int value ...)
{
	int result = 0;
	int* pvalue = &value;
	while (count--)
	{
		result += *pvalue++;
	}
	return result;
}
