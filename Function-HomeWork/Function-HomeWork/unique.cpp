#include "unique.h"
#include <cstdlib>
#include <algorithm>

int unique(int min_value, int max_value) {
	// Масив попередніх станів, та лічильник попередніх станів.
	static int *states = new int[1];
	static std::size_t counter = 0;


	// Можливість скинути лічильник, та очистити масив станів.
	if (min_value == max_value) {
		delete[] states;
		states = (min_value == 0) ? 0 : new int[1];
		counter = 0;
		return min_value - 1;
	}

	// Унікальне значення. 
	int result = 0, *found;

	/* Оскільки, від виклику до виклику, інтервал може змінитися - важко відсідкувати
	 * коли унікальні зачення закінчаться.Тому, якщо було виконано max_value - min_value + 1 спроб згенерувати
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