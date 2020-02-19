#include <iostream>
#include <time.h>

/**
 * Доречі, про рекурсію, start та end...
 * збочений алгоритм пошуку мінімального елементу, перероблений з неменш збоченого
 * алгоритму сортування рекурсивним злиттям.
 *
 * start - вказівник на перший елемент масиву,
 * end - вказівник на останній елемент масиву,
 */
template <class Type>
Type min_element(Type* start, Type* end) {
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
/**
 * Лінійний пошук максимального значення
 */
template <class Type>
Type max_element(Type* array, std::size_t len) {
	Type mx_vl = array[0];
	for (auto i = 1U; i < len; i++) {
		if (array[i] > mx_vl) 
			mx_vl = array[i];
	}
	return mx_vl;
}
/**
 * Здвигає на місці масив array, довжиною arr_len на n значень вліво, якщо left == true і заповнює нулями 
 * інтервал [array[0], array[n]),
 * 
 * вправо, якщо left == false і заповнює нулями інтервал  [ array[arr_len - n], array[arr_len] );
 *
 * left - напрям здвигу, 
 * array - масив, що здвигається,
 * arr_len - довжина масиву,
 * n - кількість елементів на які потрібно здвинути масив.
 */
template <class Type>
void shift_array(bool left, Type array[], size_t arr_len, size_t n) {
	int i = (left) ? arr_len - 1 : 0, N = (left) ? i - n : n;
	for (; (left) ? i > n - 1 : i < arr_len - n; 

		i+=(left) ? -1 : +1, 
		N+=(left) ? -1 : +1)

		array[i] = array[N];

	for (; (left) ? i >= 0 : i < arr_len;
			i += (left) ? -1 : +1)
		array[i] = 0;

}
char* to_bin_array(unsigned long long num) {
	char* result = new char[64];
	memset(result, 0, 64);
	int bit = 63;
	while (num > 0) {
		result[bit] = (char)(num & 1);
		bit -= 1;
		num >>= 1;
	}
	return result;
	
}
char* to_hex_array(unsigned long long num) {
	char* result = new char[16];
	memset(result, 0, 16);
	int bit = 15;
	while (num > 0) {
		result[bit] = (char)(num & 0xF);
		bit -= 1;
		num >>= 4;
	}
	return result;;
}

int main() {
	std::size_t size;
	std::cout << "Enter the random array size: ";
	std::cin >> size;
	srand(time(0));
//--------------	
	int* array = new int[size];
	std::cout << ((size < 1000)? "Input array is:" : "Generating input array complete!") << std::endl;
	for (std::size_t i = 0; i < size; i++) {
		array[i] = rand();
		if (size < 1000) {
			std::cout << array[i];
			if (i < size - 1) std::cout << ", ";
			else
				std::cout << ";\n";
		}
	}
	std::cout << std::endl;

	int mn_vl = min_element<int>(array, array + size), mx_vl = max_element<int>(array, size);
	std::cout << "The minimum value of the array is " << mn_vl << std::endl;
	std::cout << "The maximum value of the array is " << mx_vl << std::endl;
	std::cout << std::endl;
//--------------	
	int n;
	bool left;
	std::cout << "Tell us how much you want to shift the array? ";
	std::cin >> n;
	std::cout << "Left (1) or right (0)? ";
	std::cin >> left;
	shift_array<int>(left, array, size, n);
	std::cout << std::endl;
	for (std::size_t i = 0; i < size; i++) {
		if (size < 1000) {
			std::cout << array[i];
			if (i < size - 1) std::cout << ", ";
			else
				std::cout << ";\n";
		}
	}
	std::cout << std::endl;

//--------------	
	union {
		double user_input;
		unsigned long long bits;
	} num;
	std::cout << "DOUBLE to binary array.\nEnter the real number: ";
	std::cin >> num.user_input;
	char* ar = to_bin_array(num.bits);
	for (int i = 0; i < 64; i++) std::cout << (char)('0' + ar[i]);
	std::cout << std::endl;
	for (int i = 0; i < 64; i++) {
		if (i == 0) std::cout << "Sign = ";
		if (i == 1) std::cout << " Exponent = ";
		if (i == 12) std::cout << " Mantissa = ";
		std::cout << (char)('0' + ar[i]);
	}
	std::cout << std::endl;
	delete ar;
	std::cout << std::endl;
//-------------------
	std::cout << "DOUBLE to hex array.\nEnter the real number: ";
	std::cin >> num.user_input;
	ar = to_hex_array(num.bits);
	for (int i = 0; i < 16; i++) {
		if(ar[i] <= 9)
			std::cout << (char)('0' + ar[i]);
		else 
			std::cout << (char)('A'- 0xA + ar[i]);
	}
	delete ar;
	std::cout << std::endl;

	delete[] array;
}