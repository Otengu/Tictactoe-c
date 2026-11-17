#ifndef INPUT_H
#define INPUT_H

#include "board.h"

#define MIN_SIZE 2
#define MAX_SIZE 20
#define DEFAULT_SIZE 3

void read_board_size(int* rows, int* cols);

void read_symbol(int player_number, char* symbol, char forbidden);

int read_move(const Board* board, int player_number, char symbol, int* row, int* col);

#endif