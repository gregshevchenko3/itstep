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
 * Меняет значения местами
 */
inline void exchg(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}


/**
 * Создаёт и сортирует копию массива arr,
 * длинной length в порядке возрастания, если up == true, в порядке убывания, если up == false, и возвращает ее!
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
	std::cout << "Количество итераций: " << iter << std::endl;
	return result;
}

/**
 * Сдвигает часть масива arr, начинающуюся с элемента с индексом start, заканчивая индексом pos, на 1 вправо
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
	std::cout << "Количество итераций: " << iter << std::endl;
	return result;
}


/**
 *  Ищет минимальное значение (min == true) или максимальное значение (min == false) в масиве arr, длинной length, возвращает
 * индекс найденого элемента.
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
	std::cout << "Введите размер масива: ";
	std::cin >> ar_size;

	arr = new int[ar_size];

	for (auto i = 0; i < ar_size; i++) {
		arr[i] = std::rand();
	}
	std::cout << "Згенерированый массив:" << std::endl;
	show_arr(ar_size, arr);

	// Сортировка
	std::cout << "Сортировка метод вставки" << std::endl;
	{
		std::cout << std::endl;
		int *sort_up = sort_2(true, ar_size, arr);
		std::cout << "Отсортированный по возврастанию массив:" << std::endl;
		show_arr(ar_size, sort_up);
		delete[] sort_up;

		std::cout << std::endl;
		int *sort_down = sort_2(false, ar_size, arr);
		std::cout << "Отсортированный по убыванию массив:" << std::endl;
		show_arr(ar_size, sort_down);
		delete[] sort_down;
	}
	std::cout << std::endl << "Сортировка пузырьковый метод" << std::endl;
	{
		std::cout << std::endl;
		int *sort_up = sort_1(true, ar_size, arr);
		std::cout << "Отсортированный по возврастанию массив:" << std::endl;
		show_arr(ar_size, sort_up);
		delete[] sort_up;

		std::cout << std::endl;
		int *sort_down = sort_1(false, ar_size, arr);
		std::cout << "Отсортированный по убыванию массив:" << std::endl;
		show_arr(ar_size, sort_down);
		delete[] sort_down;
	}
	// Поиск минимального и максимального значения.
	int min = find(true, arr, ar_size), max = find(false, arr, ar_size);
	std::cout << "Минимальное значение arr[" << min << "] = " << arr[min] << std::endl;
	std::cout << "Максимальное значение arr[" << max << "] = " << arr[max] << std::endl;

	delete[] arr;

	
}