#include "array.h"

void Print(int arr[], const int length) {
	std::cout << "[ ";
	for (int i = 0; i < length; i++) {
		std::cout << arr[i] << ", ";
	}
	std::cout << "]" << std::endl;
}
void Print(double arr[], const int length) {
	std::cout << "[ ";
	for (int i = 0; i < length; i++) {
		std::cout << arr[i] << ", ";
	}
	std::cout << "]" << std::endl;
}
void Print(int arr[ROWS][COLS], const int ROWS, const int COLS)
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			std::cout << arr[i][j] << "," << "\t";
		}
		std::cout << std::endl;
	}
}
void Print(double arr[ROWS][COLS], const int ROWS, const int COLS)
{
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			std::cout << arr[i][j] << "," << "\t";
		}
		std::cout << std::endl;
	}
}

void FillRand(int arr[], const int length) {
	for (int i = 0; i < length; i++) {
		arr[i] = rand() % 100;
	}
}
void FillRand(double arr[], const int length) {
	for (int i = 0; i < length; i++) {
		arr[i] = rand() % 100;
		arr[i] /= 10;
	}
}
void FillRand(int arr[ROWS][COLS], const int ROWS, const int COLS) {
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLS; j++) {
			arr[i][j] = rand() % 100;
		}
	}
}
void FillRand(double arr[ROWS][COLS], const int ROWS, const int COLS) {
	for (auto i = 0; i < ROWS; i++) {
		for (auto j = 0; j < COLS; j++) {
			arr[i][j] = rand() % 1000;
			arr[i][j] /= 10;
		}
	}
}

void Sort(int arr[], const int length) {
	for (int i = 0; i < length; i++) {
		for (int j = i+1; j < length; j++) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}
void Sort(double arr[], const int length) {
	for (int i = 0; i < length; i++) {
		for (int j = i + 1; j < length; j++) {
			if (arr[i] > arr[j]) {
				double tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}
void Sort(int arr[ROWS][COLS], const int ROWS, const int COLS)
{
	for (auto row_i = 0; row_i < ROWS; row_i++)
	{
		for (auto col_i = 0; col_i < COLS; col_i++)
		{
			for (auto row_j = row_i; row_j < ROWS; row_j++)
			{
				for (auto col_j = (row_i == row_j)?col_i:0; col_j < COLS; col_j++)
				{
					if (arr[row_i][col_i] > arr[row_j][col_j])
					{
						int tmp = arr[row_i][col_i];
						arr[row_i][col_i] = arr[row_j][col_j];
						arr[row_j][col_j] = tmp;
					}
				}
			}

		}
	}
}

void Sort(double arr[ROWS][COLS], const int ROWS, const int COLS)
{
	for (auto row_i = 0; row_i < ROWS; row_i++)
	{
		for (auto col_i = 0; col_i < COLS; col_i++)
		{
			for (auto row_j = row_i; row_j < ROWS; row_j++)
			{
				for (auto col_j = (row_i == row_j) ? col_i : 0; col_j < COLS; col_j++)
				{
					if (arr[row_i][col_i] > arr[row_j][col_j])
					{
						double tmp = arr[row_i][col_i];
						arr[row_i][col_i] = arr[row_j][col_j];
						arr[row_j][col_j] = tmp;
					}
				}
			}

		}
	}
}
