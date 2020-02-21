#pragma once

enum menu_action {
	Exit,
	New_game,
};
void init_field();
// Комп'ютерний гравець.
void calc_comp_move();
// Перевiрка iгрового поля;
void check(bool, int);
// Хiд 
void move(bool, int);
// Отрисовка поля
void update(bool player, int move_count, int pos);
void create_rating_vector(int atoms[8]);
