#pragma once
#include <conio.h>

using enWay = enum {
	up = 72, 
	down = 80, 
	left = 75, 
	right = 77, 
	Exit = 27,
};
using enCtrl = enum {
	tab = 9,
	enter = 13,
};
enWay get_way();
enCtrl get_ctrl();