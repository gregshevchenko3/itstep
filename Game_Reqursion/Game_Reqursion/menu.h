#pragma once
#include <string>
#include "u8string.h"
#include "status_bar.h"

enum keys {
	right = 77,
	bottom = 80,
	left = 75,
	top = 72,
	enter = 13,
	escape = 27
};

static std::string bar;

struct menu_item {

	typedef enum {
		check_box, button,
	} mtype;

	mtype type;
	bool checked;
	bool selected;
	std::string title;
	int action;

	menu_item(mtype _type, bool _checked, bool _selected, std::string _title, int _action) :
		type(_type), checked(_checked), selected(_selected), title(_title), action(_action) {};
};

void menu_navigation(menu_item* menu_items, std::size_t menu_item_count);
void show_menu(menu_item* menu_items, std::size_t menu_item_count);