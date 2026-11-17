#ifndef BOARD_H
#define BOARD_H

#define EMPTY_CELL ' '

typedef struct {
	int rows;
	int cols;
	char** cells;
} Board;

#define PLACE_OK 0
#define PLACE_OUT_RANGE (-1)
#define PLACE_OCCUPIED (-2)

int board_init(Board* board, int rows, int cols);

void board_free(Board* board);

void board_print(const Board* board);

int board_place(Board* board, int row, int col, char symbol);

int board_is_full(const Board* board);

int board_check_win(const Board* board, char symbol);

#endif
