#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <windows.h>

/**
 * ROWS - ������� ����� �������
 * COLS - ������� ������� � �������
 * FIELD_WIDTH, FIELD_HEIGHT - ������ � ������ ������ � �������
 *
 * �� ����� ��������� �� ���������� ����� ��� ����� �������� ���������� -D,
 * ��� �������� ���� "table_config.h" � �����, � ��������� ���� ����� ����������� TextGraphics.h
 */

// ������ � "Error List" ������� ��� ��, �� �� �������� �������� ���������.
#if !defined(ROWS) && !defined(COLS) && !defined(FIELD_WIDTH) && !defined(FIELD_HEIGHT) && \
	!defined(SELCTED_BUTTON_ATTR) && !defined (BUTTON_ATTR) && !defined(HELP_BAR) && !defined (TEXT_ATTR)
#	error "include \"game.h\" instead \"TextGraphics.h\"
#endif


using field_to_string_t = std::string(*)(int field_content);

size_t u8length(const char* u8str);

std::string standart_field_to_string(int field_content);
std::string zero_field_is_empty(int field_content);


void text_graphics(bool init);
void drawTable(int content[ROWS][COLS], COORD offset = { 0, 0 }, field_to_string_t field_to_string = standart_field_to_string);
void drawScore(int current, int best, COORD offset);
bool drawHelpBar(std::string hlp[], int size);
bool drawYesNoMsg(std::string msg);