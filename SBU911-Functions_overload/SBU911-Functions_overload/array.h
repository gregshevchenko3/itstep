#pragma once
#include <iostream>

const int ROWS = 3;
const int COLS = 3;

void Print(int arr[], const int length);
void Print(double arr[], const int length);
void Print(int arr[ROWS][COLS], const int ROWS, const int COLS);
void Print(double arr[ROWS][COLS], const int ROWS, const int COLS);

void FillRand(int arr[], const int length);
void FillRand(double arr[], const int length);
void FillRand(int arr[ROWS][COLS], const int ROWS, const int COLS);
void FillRand(double arr[ROWS][COLS], const int ROWS, const int COLS);

void Sort(int arr[], const int length);
void Sort(double arr[], const int length);
void Sort(int arr[ROWS][COLS], const int ROWS, const int COLS);
void Sort(double arr[ROWS][COLS], const int ROWS, const int COLS);