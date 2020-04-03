#include "keypad.h"

enWay get_way()
{
	auto key = _getch();
	if (key == 226) key = _getch();
	if (key == enWay::up || key == enWay::down || key == enWay::left || key == enWay::right || key == enWay::Exit) 
		return (enWay)key;
	else
		return get_way();
}

enCtrl get_ctrl()
{
	auto key = _getch();
	if(key == enCtrl::tab || key == enCtrl::enter)
		return enCtrl(key);
	return
		get_ctrl();
}


