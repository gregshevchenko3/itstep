#include <ctime>
#include <conio.h>
#include "arrays.h"

//#define ARRAY_
//#define ARRAY_2D
//#define ARRAY_2D__ROWS_MANIPULATION
//#define ARRAY_2D__COLS_MANIPULATION


/**
 * This function will work only static array corectly, for dynamic arrays, this function allways return 1.
 */
template<typename Type>
inline size_t items_in(Type& a) {
	return sizeof a / sizeof(std::remove_extent<Type>::type);
}


void main() {
	std::srand(time(0));
	
#ifdef ARRAY_
	float sfa[10];
	int *dia = 0;
	size_t dia_size = 10;

	std::cout << "fill arrays" << std::endl;
	std::cout << "---------------------------------\n";
	FillRand(sfa, items_in(sfa), -1.0f, 1.0f);
	std::cout << "sfa = ";
	Print(sfa, items_in(sfa));

	std::cout << std::endl << std::endl << "create array" << std::endl;
	std::cout << "---------------------------------\n";
	dia = CreateRand<int>(dia_size);
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);

	std::cout << std::endl << std::endl << "append to array" << std::endl;
	std::cout << "---------------------------------\n";
	// call void push_back(Type *& arr, std::size_t & size_of_arr, Type value)
	push_back(dia, dia_size, std::numeric_limits<int>::min()); 
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);
	

	std::cout << std::endl << std::endl << "prepend to array" << std::endl;
	std::cout << "---------------------------------\n";
	// call void push_front(Type *& arr, std::size_t & size_of_arr, Type value)
	push_front(dia, dia_size, std::numeric_limits<int>::max());
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);
	
	std::cout << std::endl << std::endl << "insert std::numeric_limits<int>::min() into array for 5-th position." << std::endl;
	std::cout << "---------------------------------\n";
	// call void insert(Type *& arr, std::size_t & size_of_arr, size_t position, Type value)
	insert(dia, dia_size, 5, std::numeric_limits<int>::min());
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);

	
	std::cout << std::endl << std::endl << "remove 4-th item from array" << std::endl;
	std::cout << "---------------------------------\n";
	// call void remove(Type *& arr, std::size_t & size_of_arr, size_t position)
	remove(dia, dia_size, 4);
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);

	int i_removed;
	std::cout << std::endl << std::endl << "pop back from array" << std::endl;
	std::cout << "---------------------------------\n";

	// call Type pop_back(Type *& arr, std::size_t & size_of_arr)
	i_removed = pop_back(dia, dia_size);
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);
	std::cout << "removed value from dia[] is " << i_removed << std::endl;

	std::cout << std::endl << std::endl << "pop front from array" << std::endl;
	std::cout << "---------------------------------\n";
	// call Type pop_front(Type *& arr, std::size_t & size_of_arr)
	i_removed = pop_front(dia, dia_size);
	std::cout << "dia[" << dia_size << "] = ";
	Print(dia, dia_size);
	std::cout << "removed value from dia[] is " << i_removed << std::endl;
	delete[] dia;

#endif
	//std::cout << "Press any key..."; _getch();
#ifdef ARRAY_2D
	std::cout << std::endl << std::endl << "Create 2D array" << std::endl;
	std::cout << "---------------------------------\n";
	size_t rows = 5, cols = 5;
	float ** dfa2D = CreateRand(rows, cols, 0.1f, 1.0f);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	float inserted[5] = { 1, 2, 3, 4, 5 };
#	ifdef ARRAY_2D__ROWS_MANIPULATION
	std::cout << std::endl << std::endl << "pop back row from array" << std::endl;
	std::cout << "---------------------------------\n";
	delete [] pop_row_back(dfa2D, rows, cols);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "pop front row from array" << std::endl;
	std::cout << "---------------------------------\n";
	delete [] pop_row_front(dfa2D, rows, cols);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "append row into array" << std::endl;
	std::cout << "---------------------------------\n";
	push_row_back(dfa2D, rows, cols, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "prepend row into array" << std::endl;
	std::cout << "---------------------------------\n";
	push_row_front(dfa2D, rows, cols, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "insert row into array for 3-th position" << std::endl;
	std::cout << "---------------------------------\n";
	insert_row(dfa2D, rows, cols, 3, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "remove 2-nd row from array " << std::endl;
	std::cout << "---------------------------------\n";
	remove_row(dfa2D, rows, cols, 2);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);
#	endif
#	ifdef ARRAY_2D__COLS_MANIPULATION
	std::cout << std::endl << std::endl << "append col into array" << std::endl;
	std::cout << "---------------------------------\n";
	push_col_back(dfa2D, rows, cols, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "prepend col into array" << std::endl;
	std::cout << "---------------------------------\n";
	push_col_front(dfa2D, rows, cols, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "pop back col from array" << std::endl;
	std::cout << "---------------------------------\n";
	delete [] pop_col_back(dfa2D, rows, cols);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "pop front col from array" << std::endl;
	std::cout << "---------------------------------\n";
	delete[] pop_col_front(dfa2D, rows, cols);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "insert col into array for 2-nd position" << std::endl;
	std::cout << "---------------------------------\n";
	insert_col(dfa2D, rows, cols, 2, inserted);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);

	std::cout << std::endl << std::endl << "remove 3-nd col from array" << std::endl;
	std::cout << "---------------------------------\n";
	remove_col(dfa2D, rows, cols, 3);
	std::cout << "dfa2D[" << rows << "][" << cols << "] = ";
	Print(dfa2D, rows, cols);
#	endif
	for (auto i = 0u; i < rows; i++)
	{
		delete[] dfa2D[i];
	}
	delete[] dfa2D;
#endif
}
