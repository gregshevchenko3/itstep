#pragma once

#include <iostream>
#include <iomanip>

/**
 * Fill the array with random values between "min" and "max".
 *
 * Type - type of array
 */
template <typename type>
void FillRand(type* arr, size_t size_of_arr, type min, type max);
template <typename type>
void FillRand(type** arr, size_t rows, size_t cols, type min, type max);
/**
 * Fill the array with random values between 0 and 1 for floating point types, and between 0 and RAND_MAX
 * for integral types.
 *
 * Type - type of array
 */
template <typename Type>
void FillRand(Type* arr, std::size_t size);
template <typename Type>
void FillRand(Type** arr, size_t rows, size_t cols);
/**
 * Return the array created and filled with random values between "min" and "max".
 *
 * Type - type of array
 */
template <typename type>
type* CreateRand(std::size_t size, type min, type max);
template <typename type>
type** CreateRand(size_t rows, size_t cols, type min, type max);
/**
 * Return the array created and filled with random values between 0 and 1 for floating point types, and between 0 and RAND_MAX
 * for integral types.
 *
 * Type - type of array
 */
template <typename Type>
Type* CreateRand(const std::size_t size);
template <typename Type>
Type** CreateRand(size_t rows, size_t cols);
/** Display the array */
template <typename Type>
void Print(Type* arr, std::size_t size);
template <typename Type>
void Print(Type** arr, size_t rows, size_t cols);

/**
 * Inserts the "value" element into the specified "position" of the "arr" array 
 * and increases the array size (size_of_arr) by 1.
 */
template <typename Type>
void insert(Type *& arr, std::size_t & size_of_arr, std::size_t position, Type value);
template <typename Type>
void insert_row(Type **& arr, size_t& rows, size_t& cols, std::size_t row_position, Type* row);
template <typename Type>
void insert_col(Type **& arr, size_t& rows, size_t& cols, std::size_t col_position, Type* col);
/**
 * Removes an item at the specified position of array, reduces the size of array (size_of_arr) by 1, 
 */
template <typename Type>
void remove(Type *& arr, std::size_t & size_of_arr, size_t position);
template <typename Type>
void remove_row(Type **& arr, size_t& rows, size_t& cols, std::size_t row_position);

template <typename Type>
void remove_col(Type **& arr, size_t& rows, size_t& cols, std::size_t col_position);
/**
 * Append a item "value" (or "row", or col, for 2-dimension array) to array "arr"
 */
template <typename Type>
void push_back(Type*& arr, std::size_t& size_of_arr, Type value);
template <typename Type>
void push_row_back(Type**& arr, std::size_t& rows, size_t& cols, Type* row);
template <typename Type>
void push_col_back(Type**& arr, std::size_t& rows, size_t& cols, Type* col);
/**
 * Prepend a item "value" (or "row", or col, for 2-dimension array) to array "arr"
 */
template <typename Type>
void push_front(Type*& arr, std::size_t& size_of_arr, Type value);
template <typename Type>
void push_row_front(Type**& arr, std::size_t& rows, size_t& cols, Type* row);
template <typename Type>
void push_col_front(Type**& arr, std::size_t& rows, size_t& cols, Type* col);
/*
 * Removes an item at the end of array, reduces the size of array (size_of_arr) by 1, 
 * and returns the removed value
 */
template<typename Type>
Type pop_back(Type *& arr, std::size_t & size_of_arr);
template <typename Type>
Type* pop_row_back(Type**& arr, std::size_t& rows, size_t& cols);
template <typename Type>
Type* pop_col_back(Type**& arr, std::size_t& rows, size_t& cols);
/*
 * Removes an item at the begin of array, reduces the size of array (size_of_arr) by 1,
 * and returns the removed value
 */
template<typename Type>
Type pop_front(Type *& arr, std::size_t & size_of_arr);
template <typename Type>
Type* pop_row_front(Type**& arr, std::size_t& rows, size_t& cols);
template <typename Type>
Type* pop_col_front(Type**& arr, std::size_t& rows, size_t& cols);

#include "arrays.impl.h"
