#include <iostream>
#include <clocale>

/**
 * Функція-генератор унікальних випадкових чисел від min_value, до max_value.
 * 
 * Якщо передати min_value та max_value, такі, що min_value == max_value, то це приведе до сидання 
 * внутрішнього масиву станів.
 *
 * Повертає:
 * значення в межах [min_value, max_value], 
 * 
 * min_value-1, якщо не можливо згенерувати унікальне значення, або у випадку зкидання внутрішнього масиву станів.
 */
int unique(int min_value, int max_value);
/**
 *  Заповнює масив унікальними значеннями. Послідовно, з початку масиву.
 *
 *  Повертає кількість унікальних (що не поторюються) значень в масиві, остальні елементи, для яких не вдалося згенерувати 
 *  унікальне випадкове число - заповнюються значенням min_value - 1;
 */
int fills_with_unique(int array[], const size_t length, int min_value, int max_value);
/** Середнє арифметичне елементфв масиву */
double avg(int array[], const size_t length);
/** Copy-paste з попередньої домашки*/
template <class Type> Type max_element(Type* array, std::size_t len); // лінійний алгоритм
template <class Type> Type min_element(Type* start, Type* end); // рекурсивний алгоритм
void sort(const bool up, int length, int arr[]);
/**
 * Вивід на екран масиву 
 */
void print_array(int array[], const size_t length) {
	std::cout << "[";
	for (auto i = 0; i < length; i++) std::cout << array[i] << ", ";
	std::cout << "]" << std::endl;
}

#include <ctime>

void main() {
	std::setlocale(LC_ALL, "en_US.utf8");
	srand(time(0));

	const size_t arr_len = 10;

	int arr[arr_len];
	int res_len = fills_with_unique(arr, arr_len, -10, 10);
	std::cout << u8"Масив з " << arr_len << u8" унiкальних випадкових значеннь в межах вiд -10 до 10:" << std::endl;
	if (arr_len != res_len) {
		std::cout << u8"ПОМИЛКА: В процесi генерацiї унiкальних значень вiдбулася помилка, тому згенеровано лише " << res_len << u8" елементiв" << std::endl;
	}
	print_array(arr, res_len);
	std::cout << std::endl;

	std::cout << u8"Середнє арифметичне згенерованого масиву: " << avg(arr, res_len) << std::endl;
	std::cout << std::endl;

	std::cout << u8"Найменше значення в масивi: " << min_element<int>(arr, arr + res_len) << std::endl;
	std::cout << std::endl;

	std::cout << u8"Найбiльше значення в масивi: " << max_element<int>(arr, res_len) << std::endl;
	std::cout << std::endl;

	std::cout << u8"Вiдсортований масив (в порядку зростання):" << std::endl;
	sort(true, res_len, arr);
	print_array(arr, res_len);
	std::cout << std::endl;

	std::cout << u8"Вiдсортований масив (в порядку спадання):" << std::endl;
	sort(false, res_len, arr);
	print_array(arr, res_len);
	std::cout << std::endl;

	// Внутрішній буфер станів
	unique(0, 0);
}

int unique(int min_value, int max_value) {
	// Масив попередніх станів, та лічильник попередніх станів.
	static int *states = new int[1];
	static std::size_t counter = 0;

	// Можливість скинути лічильник, та очистити масив станів.
	if (min_value == max_value) {
		delete[] states;
		states = new int[1];
		counter = 0;
		return min_value - 1;
	}

	// Унікальне значення. 
	int result = 0, *found;

	/* Оскільки, від виклику до виклику, інтервал може змінитися - важко відсідкувати
	 * коли унікальні зачення закінчаться.Тому, якщо було виконано max_value - min_value спроб згенерувати
	 * випадкове значення, а воно все ще не унікальне то унікальних значень уже й не буде.
	 */
	int tmp_counter = 2 * (max_value - min_value);
	do {
		result = std::rand() % (max_value - min_value + 1) + min_value;
		found = std::find(states, states + counter, result);
	} while (found != states + counter && tmp_counter-- >= 0);
	if (tmp_counter < 0) return min_value - 1;
	states[counter++] = result;

	// Збільшую кількість місця в масиві.
	int* new_states = new int[counter + 1];
	std::copy(states, states + counter, new_states);
	new_states[counter] = 0;
	delete[] states;
	states = new_states;

	return result;
}
int fills_with_unique(int array[], size_t length, int min_value, int max_value) {
	int result = 0;
	for (auto i = 0; i < length; i++) {
		array[i] = unique(min_value, max_value);
		if (array[i] >= min_value) result++;
		else
			break;
	}
	return result;
}

double avg(int array[], const size_t length) {
	double sum = 0; 
	for (auto i = 0; i < length; i++) {
		sum += array[i];
	}
	return sum / length;
}

template <class Type> Type min_element(Type* start, Type* end) {
	std::size_t len = end - start;
	if (len == 2) {
		if (*start < *(start + 1))
			return *start;
		else
			return *(start + 1);

	}
	else if (len == 1)
		return *start;
	else {
		Type e1 = min_element<Type>(start, start + len / 2);
		Type e2 = min_element<Type>(start + len / 2, end);

		if (e1 < e2) return e1;
		else return e2;

	}
}

template <class Type> Type max_element(Type* array, std::size_t len) {
	Type mx_vl = array[0];
	for (auto i = 1U; i < len; i++) {
		if (array[i] > mx_vl)
			mx_vl = array[i];
	}
	return mx_vl;
}
void sort(const bool up, int length, int arr[]) {
	int iter = 0;
	for (auto i = 1; i < length; i++) {
		auto item = arr[i];
		int j = i;
		for (; j > 0 && ((up) ? (arr[j - 1] > item) : (arr[j - 1] < item)); j--)
			arr[j] = arr[j - 1];
		arr[j] = item;
		iter++;
	}
}
