#include <iostream>
#include <ctime>
#include "array.h"

#define delimiter std::cout << "\n--------------------\n"

void main() {
	const int n = 5, m = 10;
	int arr[n], brr[m];

	srand(time(0));

	FillRand(arr, n);
	Print(arr, n);
	Sort(arr, n);
	Print(arr, n);
	delimiter;

	FillRand(brr, m);
	Print(brr, m);
	Sort(brr, m);
	Print(brr, m);
	delimiter;

	double crr[m];
	FillRand(crr, m);
	Print(crr, m);
	Sort(crr, m);
	Print(crr, m);
	delimiter;

	int arr2D[ROWS][COLS] = {0};
	FillRand(arr2D, ROWS, COLS);
	Print(arr2D, ROWS, COLS);
	std::cout << std::endl;
	Sort(arr2D, ROWS, COLS);
	Print(arr2D, ROWS, COLS);
	delimiter;

	double brr2D[ROWS][COLS] = { 0 };
	FillRand(brr2D, ROWS, COLS);
	Print(brr2D, ROWS, COLS);
	std::cout << std::endl;
	Sort(brr2D, ROWS, COLS);
	Print(brr2D, ROWS, COLS);
	delimiter;
}