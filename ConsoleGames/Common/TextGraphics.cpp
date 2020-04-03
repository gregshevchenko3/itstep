#if !defined(ROWS) && !defined(COLS) && !defined(FIELD_WIDTH) && !defined(FIELD_HEIGHT) && \
	!defined(SELCTED_BUTTON_ATTR) && !defined (BUTTON_ATTR) && !defined(HELP_BAR) && !defined (TEXT_ATTR)
#include "table_config.h"
#endif

#include "TextGraphics.h"
#include "keypad.h"
#include <vector>

void text_graphics(bool init)
{
	static int old_attr = 0;
	if (init) {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		old_attr = info.wAttributes;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TEXT_ATTR);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), old_attr);
		std::system("cls");
	}
}

void drawTable(int content[ROWS][COLS], COORD offset, field_to_string_t field_to_string)
{
	static bool once = true;
	static std::string table_horizon_cell_border = "", cell_horizon_internal_border;

	const char
		*table_top_left_border = u8"\u2554",			// Верхній лівий кут
		*table_horizontal_border = u8"\u2550",				// Горизонтальна подвійна лінія
		*cell_in_top_row_top_left_top_right_border = u8"\u2564", // Верхній правий та лівий кути ячейок у верхній строці
		*table_top_right_border = u8"\u2557",					// Верхній правий кут

		*table_bottom_left_border = u8"\u255A",				// Нижній лівий кут
		*cell_in_bottom_row_bottom_left_bottom_right_border = u8"\u2567",		// Нижні правий та лівий кути ячейок у нижній строці
		*table_bottom_right_border = u8"\u255d",				// нижній правий кут

		*cell_in_left_colomn_top_left_bottom_left_border = u8"\u255f",					// верхній та нижній кути ячейок в лівому стовпці
		*cell_internal_horizontal_border = u8"\u2500",
		*cell_internal_angle_border = u8"\u253c",
		*cell_in_right_column_top_right_bottom_right_border = u8"\u2562",
		*table_vertical_border = u8"\u2551",
		*cell_internal_vertical_border = u8"\u2502";

	if (once) {
		for (int i = 0; i < FIELD_WIDTH + 2; i++) {
			table_horizon_cell_border = table_horizon_cell_border + table_horizontal_border;
			cell_horizon_internal_border = cell_horizon_internal_border + cell_internal_horizontal_border;
		}
		once = false;
	}
	COORD coord = { offset.X, offset.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	for (auto r = 0; r <= FIELD_HEIGHT * ROWS + ROWS; r++)
	{
		for (auto c = 0; c <= 2 * COLS; c++)
		{
			if (c == 0)
				if (r == 0)
					std::cout << table_top_left_border;
				else if (r == FIELD_HEIGHT * ROWS + ROWS)
					std::cout << table_bottom_left_border;
				else if (r % (FIELD_HEIGHT + 1))
					std::cout << table_vertical_border;
				else
					std::cout << cell_in_left_colomn_top_left_bottom_left_border;
			else if (c == 2 * COLS)
				if (r == 0)
					std::cout << table_top_right_border;
				else if (r == FIELD_HEIGHT * ROWS + ROWS)
					std::cout << table_bottom_right_border;
				else if (r % (FIELD_HEIGHT + 1))
					std::cout << table_vertical_border;
				else
					std::cout << cell_in_right_column_top_right_bottom_right_border;
			else if (c & 1)
				if (r == 0)
					std::cout << table_horizon_cell_border;
				else if (r == FIELD_HEIGHT * ROWS + ROWS)
					std::cout << table_horizon_cell_border;
				else if (r % (FIELD_HEIGHT + 1))
					if (r % (FIELD_HEIGHT + 1) == FIELD_HEIGHT / 2 + 1)
						std::cout << " " << std::setw(FIELD_WIDTH) << field_to_string(content[r / (FIELD_HEIGHT + 1)][c / 2]) << " ";
					else
						std::cout << " " << std::setw(FIELD_WIDTH) << " " << " ";
				else
					std::cout << cell_horizon_internal_border;
			else
				if (r == 0)
					std::cout << cell_in_top_row_top_left_top_right_border;
				else if (r == FIELD_HEIGHT * ROWS + ROWS)
					std::cout << cell_in_bottom_row_bottom_left_bottom_right_border;
				else if (r % (FIELD_HEIGHT + 1))
					std::cout << cell_internal_vertical_border;
				else
					std::cout << cell_internal_angle_border;
		}
		coord.Y++;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
}
void drawScore(int current, int best, COORD offset)
{
	COORD coord = { offset.X, offset.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "Score: " << current;
	coord.Y++;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << "Best: " << best;
}
bool drawHelpBar(std::string hlp[], int size)
{
	short Y;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	auto len = 0L;
	for (auto i = 0; i < size; i++)
	{
		len += u8length(hlp[i].c_str()) + std::cout.width();
	}
	if (len >= csbi.srWindow.Right) return false;

	COORD coord = { 0, csbi.srWindow.Bottom };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	short old_attr = csbi.wAttributes;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), HELP_BAR);

	for (auto i = 0; i < size; i++)
	{
		std::cout << hlp[i] << "\t";
	}
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int width = csbi.dwCursorPosition.X;
	for (auto i = width; i < csbi.srWindow.Right; i++)
	{
		std::cout << " ";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), old_attr);
	return true;
}
bool drawYesNoMsg(std::string msg, bool internal)
{
	static bool select = false;
	static std::vector<std::string> lines;
	static short old_attr;
		
	
	CONSOLE_SCREEN_BUFFER_INFO info;
	auto HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(HANDLE, &info);

	if (!internal) {
		// Кількість строк в повідомленні.
		lines.erase(lines.begin(), lines.end());
		bool select = true;
		auto prev = msg.begin();
		for (auto it = msg.begin(); it != msg.end(); it++)
		{
			if (*it == '\n') {
				lines.push_back(std::string(prev, it));
				prev = ++it;
			}
		}
		lines.push_back(std::string(prev, msg.end()));
		old_attr = info.wAttributes;
	}

	// Вивод на екран.
	COORD position = { 0, (info.srWindow.Bottom - lines.size())/2 };
	for (auto it = lines.begin(); it != lines.end(); it++) {
		position.X = (info.srWindow.Right - u8length(it->c_str())) / 2;
		SetConsoleCursorPosition(HANDLE, position);
		std::cout << *it;
		position.Y++;
	}
	position.Y++;
	SetConsoleCursorPosition(HANDLE, position);
	std::cout << "  " ;
	if(select) SetConsoleTextAttribute(HANDLE, SELECTED_BUTTON_ATTR);
	else
		SetConsoleTextAttribute(HANDLE, BUTTON_ATTR);
	std::cout << u8" Так ";
	SetConsoleTextAttribute(HANDLE, old_attr);
	std::cout << "\t";
	if(!select) SetConsoleTextAttribute(HANDLE, SELECTED_BUTTON_ATTR);
	else
		SetConsoleTextAttribute(HANDLE, BUTTON_ATTR);
	std::cout << u8" Нi ";
	SetConsoleTextAttribute(HANDLE, old_attr);

	// HelpBar
	std::string hlp[] = { u8"Tab = Вибiр", u8"Enter = Натиснути" };
	drawHelpBar(hlp, 2);
	SetConsoleCursorPosition(HANDLE, {info.srWindow.Right, info.srWindow.Bottom});
	// Клавіатура
	auto ctrl = get_ctrl();
	if (ctrl == enCtrl::tab) {
		select = !select;
		return drawYesNoMsg(msg, true);
	}
	else if(ctrl == enCtrl::enter){
		return select;
	}
}
bool drawYesNoMsg(std::string msg) {
	return drawYesNoMsg(msg, false);
}
std::string standart_field_to_string(int field_content)
{
	std::stringstream ss;
	ss << field_content;
	return std::string(ss.str());
};
std::string zero_field_is_empty(int field_content)
{
	if (field_content == 0) return " ";
	std::stringstream ss;
	ss << field_content;
	return ss.str();
}
size_t u8length(const char* u8str) {
	const char u8ch_begin = ((1 << 7) | (1 << 6));
	size_t result = 0;
	const char* begin = u8str;
	while (*begin) {
		if (*begin & (1 << 7)) {
			if (*begin & u8ch_begin == u8ch_begin) {
				// Початок багатобайтового символу - як мініму два старші біти будуть встановлені
				result++;
			}
		}
		else {
			// однобайтовий символ
			result++;
		}
		begin++;
	}
	return result;
}

