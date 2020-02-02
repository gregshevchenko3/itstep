#include <iostream>
#include <ctime>
#include <cstdlib>


void show_arr(int length, int arr[]) {
	for (auto index = 0; index < length; index++)
		std::cout << arr[index] << " ";
	std::cout << std::endl;
}
void copy_arr(int length, int src[], int dst[]) {
	auto size = length;
	while (size >= 0) {
		dst[size] = src[--size];
	}
}
/**
 * ������ �������� �������
 */
inline void exchg(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}


/**
 * ������ � ��������� ����� ������� arr,
 * ������� length � ������� �����������, ���� up == true, � ������� ��������, ���� up == false, � ���������� ��!
 */
int* sort_1(const bool up, int length, int arr[]) {
	int *result = new int[length];
	copy_arr(length, arr, result);

	int iter = 0;
	for (auto i = 0; i < length; i++) {
		for (auto j = 0; j < length - 1; j++) {
			int tmp;
			iter++;
			if (up) {
				if (result[i] < result[j]) exchg(result[i], result[j]);
			}
			else {
				if (result[i] > result[j]) exchg(result[i], result[j]);
			}
		}
	}
	std::cout << "���������� ��������: " << iter << std::endl;
	return result;
}

/**
 * �������� ����� ������ arr, ������������ � �������� � �������� start, ���������� �������� pos, �� 1 ������
 */
void shift(int start, int pos, int* arr) {
	
}
int* sort_2(const bool up, int length, int arr[]) {
	int *result = new int[length];
	copy_arr(length, arr, result);

	int iter = 0;
	for (auto i = 1; i < length; i++) {
		auto item = result[i];
		int j = i;
		for (; j > 0 && ((up)?(result[j - 1] > item):(result[j - 1] < item)); j--) 
			result[j] = result[j - 1];
		result[j] = item;
		iter++;
	}
	std::cout << "���������� ��������: " << iter << std::endl;
	return result;
}


/**
 *  ���� ����������� �������� (min == true) ��� ������������ �������� (min == false) � ������ arr, ������� length, ����������
 * ������ ��������� ��������.
 */
int find(bool min, int* arr, int length) {
	int result = 0;
	for (auto i = 1; i < length; i++) {
		result = (min) ? ((arr[i] < arr[result]) ? i : result) : ((arr[i] > arr[result]) ? i : result);
	}
	return result;
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "rus");
	std::srand(std::time(0));

	int *arr;
	unsigned int ar_size = 0;
	std::cout << "������� ������ ������: ";
	std::cin >> ar_size;

	arr = new int[ar_size];

	for (auto i = 0; i < ar_size; i++) {
		arr[i] = std::rand();
	}
	std::cout << "�������������� ������:" << std::endl;
	show_arr(ar_size, arr);

	// ����������
	std::cout << "���������� ����� �������" << std::endl;
	{
		std::cout << std::endl;
		int *sort_up = sort_2(true, ar_size, arr);
		std::cout << "��������������� �� ������������ ������:" << std::endl;
		show_arr(ar_size, sort_up);
		delete[] sort_up;

		std::cout << std::endl;
		int *sort_down = sort_2(false, ar_size, arr);
		std::cout << "��������������� �� �������� ������:" << std::endl;
		show_arr(ar_size, sort_down);
		delete[] sort_down;
	}
	std::cout << std::endl << "���������� ����������� �����" << std::endl;
	{
		std::cout << std::endl;
		int *sort_up = sort_1(true, ar_size, arr);
		std::cout << "��������������� �� ������������ ������:" << std::endl;
		show_arr(ar_size, sort_up);
		delete[] sort_up;

		std::cout << std::endl;
		int *sort_down = sort_1(false, ar_size, arr);
		std::cout << "��������������� �� �������� ������:" << std::endl;
		show_arr(ar_size, sort_down);
		delete[] sort_down;
	}
	// ����� ������������ � ������������� ��������.
	int min = find(true, arr, ar_size), max = find(false, arr, ar_size);
	std::cout << "����������� �������� arr[" << min << "] = " << arr[min] << std::endl;
	std::cout << "������������ �������� arr[" << max << "] = " << arr[max] << std::endl;

	delete[] arr;

	
}