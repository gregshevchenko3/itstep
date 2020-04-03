#pragma once

#if !defined(ROWS) && !defined(COLS) && !defined(FIELD_WIDTH) && !defined(FIELD_HEIGHT) && \
	!defined(SELCTED_BUTTON_ATTR) && !defined (BUTTON_ATTR) && !defined(HELP_BAR) && !defined (TEXT_ATTR)
#	error "Define the preprocessor constant ROWS, COLS, FIELD_WIDTH, and FIELD_HEIGHT before including TextGraphics.h \
or create file table_config.h that contains this constans, and include it before game.h"
#endif
#include "TextGraphics.h"

bool draw(int content[ROWS][COLS], int& score, int& best);
bool move(int content[ROWS][COLS], int& score, int& best);
bool generate(int content[ROWS][COLS], int& score, int& best, bool entry = false);
bool check(int content[ROWS][COLS]);

void game_score_serialize(std::string filename, int score);
int game_score_deserialize(std::string filename);