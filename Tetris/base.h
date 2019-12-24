#ifndef __BASE_H__
#define __BASE_H__

#include <iostream>
#include <time.h>
#include <string>
#include <windows.h>
#include "exceptions.h"

	enum BlockType
	{
		I_BLOCK = 1,
		J_BLOCK = 2,
		L_BLOCK = 3,
		S_BLOCK = 4,
		Z_BLOCK = 5,
		T_BLOCK = 6,
		O_BLOCK = 7	
	};

	enum Arrow
	{
		UP = 72,
		DOWN = 80,
		LEFT = 75,
		RIGHT = 77
	};

	void gotoxy(int, int);

#endif