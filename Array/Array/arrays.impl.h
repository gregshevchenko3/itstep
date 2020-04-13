#pragma once
#include <algorithm>
#include "arrays.h"
template<typename type>
void FillRand(type * arr, std::size_t size, type min, type max)
{
	for (auto i = 0UL; i < size; i++) {
		arr[i] = std::rand() % (max - min + 1) + min;
	}
}
template<>
void FillRand<float>(float * arr, std::size_t size, float min, float max)
{
	for (auto i = 0UL; i < size; i++) {
		arr[i] = arr[i] = std::rand() * (max - min) / RAND_MAX + min;
	}
}
template<>
void FillRand<double>(double * arr, std::size_t size, double min, double max)
{
	for (auto i = 0ul; i < size; i++) {
		arr[i] = arr[i] = std::rand() * (max - min) / RAND_MAX + min;
	}
}
template<typename type>
void FillRand(type ** arr, size_t rows, size_t cols, type min, type max)
{
	for (auto i = 0u; i < rows; i++) {
		FillRand<type>(arr[i], cols, min, max);
	}
}
template<typename Type>
void FillRand(Type * arr, std::size_t size)
{
	for (auto i = 0UL; i < size; i++) {
		arr[i] = std::rand();
	}
}
template<>
void FillRand<float>(float * arr, std::size_t size)
{
	for (auto i = 0UL; i < size; i++) {
		arr[i] = std::rand() * 1.f / RAND_MAX;
	}
}
template<>
void FillRand<double>(double * arr, std::size_t size)
{
	for (auto i = 0UL; i < size; i++) {
		arr[i] = std::rand() * 1. / RAND_MAX;
	}
}
template<typename Type>
void FillRand(Type ** arr, size_t rows, size_t cols)
{
	for (auto i = 0u; i < rows; i++) {
		FillRand<Type>(arr[i], cols);
	}
}
template<typename type>
type * CreateRand(std::size_t size, type min, type max)
{
	type* result = new type[size];
	FillRand<type>(result, size, min, max);
	return result;
}
template<typename type>
type ** CreateRand(size_t rows, size_t cols, type min, type max)
{
	type** result = new type*[rows];
	for (int i = 0; i < rows; i++) {
		result[i] = new type[cols];
		FillRand<type>(result[i], cols, min, max);
	}
	return result;
}
template<typename Type>
Type * CreateRand(std::size_t size)
{
	Type* result = new Type[size];
	FillRand<Type>(result, size);
	return result;
}

template<typename Type>
Type ** CreateRand(size_t rows, size_t cols)
{
	Type** result = new Type*[rows];
	for (int i = 0u; i < rows; i++) {
		result[i] = new Type[cols];
		FillRand<Type>(result[i], cols);
	}
	return result;
}

template<typename Type>
void Print(Type * arr, std::size_t size)
{
	if (arr == nullptr) {
		std::cout << "NULL";
		return;
	}
	std::cout << "[";
	for (auto i = 0UL; i < size; i++) {
		std::cout << arr[i] << ", ";
	}
	std::cout << "]" << std::endl;
}

template<typename Type>
void Print(Type ** arr, size_t rows, size_t cols)
{
	std::cout << "{" << std::endl;
	for (auto i = 0u; i < rows; i++)
	{ 
		std::cout << "\t{";
		for (auto j = 0u; j < cols; j++)
		{
			std::cout.width(7);
			std::cout << std::setprecision(4) << arr[i][j] << ",";
		}
		std::cout << "}," << std::endl;
	}
	std::cout << "};" << std::endl;
}

template<typename Type>
void insert(Type *& arr, std::size_t & size_of_arr, std::size_t position, Type value)
{
	if (position > size_of_arr) return;
	Type* tmp = arr;
	arr = new Type[++size_of_arr];
	for (auto i = 0u; i < size_of_arr; i++) {
		if (i == position) 
		{
			arr[i] = value;
		}
		if (i >= position) {
			arr[i + 1] = tmp[i];
		}
		if( i < position)
			arr[i] = tmp[i];
	}
	delete[] tmp;
}

template<typename Type>
void insert_row(Type **& arr, size_t & rows, size_t & cols, std::size_t row_position, Type * row)
{
	Type* new_row = new Type[cols];
	std::copy(row, row + cols, new_row);
	insert<Type*>(arr, rows, row_position, new_row);
}

template<typename Type>
void insert_col(Type **& arr, size_t & rows, size_t & cols, std::size_t col_position, Type * col)
{
	for (auto i = 0u; i < rows; i++) 
	{
		insert(arr[i], cols, col_position, col[i]);
		cols--;
	}
	cols++;
}

template<typename Type>
void remove(Type *& arr, std::size_t & size_of_arr, size_t position)
{
	if (position >= size_of_arr) return;
	Type* tmp = arr;
	arr = new Type[--size_of_arr];
	for (auto i = 0u; i <= size_of_arr; i++) {
		if (i == position) continue;
		if (i < position) {
			arr[i] = tmp[i];
		}
		else {
			arr[i - 1] = tmp[i];
		}
	}
	delete[] tmp;
}

template<typename Type>
void remove_row(Type **& arr, size_t & rows, size_t & cols, std::size_t row_position)
{
	if (row_position >= rows) return;
	delete[]  arr[row_position];
	remove<Type*>(arr, rows, row_position);
}
template<typename Type>
void remove_col(Type **& arr, size_t & rows, size_t & cols, std::size_t col_position)
{
	if (col_position >= cols) return;
	for (auto i = 0u; i < rows; i++)
	{
		remove<Type>(arr[i], cols, col_position);
		cols++;
	}
	cols--;
}
template<typename Type>
void push_back(Type *& arr, std::size_t & size_of_arr, Type value)
{
	insert(arr, size_of_arr, size_of_arr, value);
}
template<typename Type>
void push_row_back(Type **& arr, std::size_t & rows, size_t & cols, Type * row)
{
	insert_row(arr, rows, cols, rows, row);
}
template<typename Type>
void push_col_back(Type **& arr, std::size_t & rows, size_t & cols, Type * row)
{
	insert_col(arr, rows, cols, cols, row);
}
template<typename Type>
void push_front(Type *& arr, std::size_t & size_of_arr, Type value)
{
	insert(arr, size_of_arr, 0, value);
}
template<typename Type>
void push_row_front(Type **& arr, std::size_t & rows, size_t & cols, Type * row)
{
	insert_row(arr, rows, cols, 0, row);
}
template<typename Type>
void push_col_front(Type **& arr, std::size_t & rows, size_t & cols, Type * col)
{
	insert_col(arr, rows, cols, 0, col);
}
template<typename Type>
Type pop_back(Type *& arr, std::size_t & size_of_arr)
{
	Type result = arr[size_of_arr - 1];
	remove(arr, size_of_arr, size_of_arr-1);
	return result;
}
template<typename Type>
Type * pop_row_back(Type **& arr, std::size_t & rows, size_t & cols)
{
	Type* result = new Type[cols];
	std::copy(arr[rows - 1], arr[rows - 1] + cols, result);
	remove_row(arr, rows, cols, rows - 1);
	return result;
}

template<typename Type>
Type * get_col_at(Type **& arr, std::size_t & rows, size_t & cols, size_t col_index) {
	Type* result = new Type[rows];
	for (auto i = 0u; i < rows; i++)
	{
		result[i] = arr[i][cols - 1];
	}
	return result;
}


template<typename Type>
Type * pop_col_back(Type **& arr, std::size_t & rows, size_t & cols)
{
	Type* result = get_col_at(arr, rows, cols, cols - 1);
	remove_col(arr, rows, cols, cols - 1);
	return result;
}
template<typename Type>
Type pop_front(Type *& arr, std::size_t & size_of_arr)
{
	Type result = arr[0];
	remove(arr, size_of_arr, 0);
	return result;
}

template<typename Type>
Type * pop_row_front(Type **& arr, std::size_t & rows, size_t & cols)
{
	Type* result = new Type[cols];
	std::copy(arr[0], arr[0] + cols, result);
	remove_row(arr, rows, cols, 0);
	return result;
}

template<typename Type>
Type * pop_col_front(Type **& arr, std::size_t & rows, size_t & cols)
{
	Type* result = get_col_at(arr, rows, cols, 0);
	remove_col(arr, rows, cols, 0);
	return result;
}

