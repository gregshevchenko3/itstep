#include <iostream>

void FillRand(int arr[], int size);

void main()
{
	int n; // Size of array;
	std::cout << "Enter the size of array: "; std::cin >> n;

	int* arr = new int[n]{3, 5, 8};

	for (int i = 0; i < n; i++) {
		std::cout << arr[i] << "\t";
	}
	std::cout << std::endl;

	delete[] arr;
}

void FillRand(int arr[], int size)
{
	
}
