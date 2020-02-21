#include "menu.h"
#include "conio.h"
#include "game.h"
#include "console.h"

void menu_navigation(menu_item* menu_items, std::size_t menu_item_count) {
	const int arrows_keypad = 224;

	while (true) {
		show_menu(menu_items, menu_item_count);
		int key = _getch();
		int chng = 0;
		auto tmp = menu_items;
		while (!menu_items->selected && (menu_items < menu_items + menu_item_count)) {
			menu_items++;
		}
		if (key == arrows_keypad) {
			key = _getch();
			switch (key) {
			case top:
				chng--;
				break;
			case bottom:
				chng++;
				break;
			}
		}
		else {
			switch (key) {
			case enter:
				switch (menu_items->action) {
				case Exit:
					exit(0);
				case New_game:
					init_field();
					update(true, 9, -1);
				}
				break;
			case escape:
				exit(0);
			}
		}
		menu_items->selected = false;
		menu_items += chng;

		if (menu_items < tmp) menu_items = tmp + menu_item_count - 1;
		if (menu_items == tmp + menu_item_count) menu_items = tmp;

		menu_items->selected = true;
		menu_items = tmp;
	}
}
// Виводе меню
void show_menu(menu_item* menu_items, std::size_t menu_item_count) {
	console_ostream<char, std::char_traits<char>> console(std::cout.rdbuf());
	console << attr(0);
	system("cls");
	coord2d viewport_size = console.get_console_size();
	auto starty = (viewport_size.y - menu_item_count) / 2;

	short menu_width = u8length(menu_items[0].title);
	for (auto i = 0; i < menu_item_count; i++) {
		short len = u8length(menu_items[i].title);
		if (len > menu_width) menu_width = len;
	}
	menu_width += 2;

	auto startx = (viewport_size.x - menu_width) / 2;
	for (auto i = 0; i < menu_item_count; i++) {
		auto char_attr = attr(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		if (menu_items[i].selected)
			char_attr = attr(BACKGROUND_INTENSITY);

		auto title_len = u8length(menu_items[i].title);
		short after = menu_width - title_len - (menu_width - title_len) / 2;
		short before = (menu_width - title_len) / 2;

		console << coord2d(startx, starty++) << char_attr;
		for (; before >= 0; before--) console << ' ';
		console << menu_items[i].title;
		for (; after >= 0; after--)  console << ' ';
	}
	show_status_bar(u8" \u2195 навiгацiя, Enter вибiр, Esc вихiд ");
}
