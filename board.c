#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNING
#endif

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int board_init(Board* board, int rows, int cols)
{
	int i, j;

	board->rows = rows;
	board->cols = cols;
	board->cells = (char**)malloc((size_t)rows * sizeof(char*));
	if (board->cells == NULL)
		return 0;

	for (i = 0; i < rows; i++) {
		board->cells[i] = (char*)malloc((size_t)cols * sizeof(char));
		if (board->cells[i] == NULL) {
			while (--i >= 0)
				free(board->cells[i]);
			free(board->cells);
			board->cells = NULL;
			return 0;
		}
		for (j = 0; j < cols; j++)
			board->cells[i][j] = EMPTY_CELL;
	}
	return 1;
}

void board_free(Board* board)
{
	int i;

	if (board->cells == NULL)
		return;
	for (i = 0; i < board->rows; i++)
		free(board->cells[i]);
	free(board->cells);
	board->cells = NULL;
}

static void print_separator(int cols)
{
	int j;

	printf("   +");
	for (j = 0; j < cols; j++)
		printf("---+");
	printf("\n");
}

void board_print(const Board* board)
{
	int i, j;

	printf("\n    ");
	for (j = 0; j < board->cols; j++)
		printf("%-4c", 'A' + j);
	printf("\n");

	print_separator(board->cols);
	for (i = 0; i < board->rows; i++) {
		printf("%2d |", i + 1);
		for (j = 0; j < board->cols; j++)
			printf("% c | ", board->cells[i][j]);
		printf("\n");
		print_separator(board->cols);
	}
	printf("\n");
}

int board_place(Board* board, int row, int col, char symbol)
{
	if (row < 0 || row >= board->rows || col < 0 || col >= board->cols)
		return PLACE_OUT_RANGE;
	if (board->cells[row][col] != EMPTY_CELL)
		return PLACE_OCCUPIED;

	board->cells[row][col] = symbol;
	return PLACE_OK;
}

int board_is_full(const Board* board)
{
	int i, j;

	for (i = 0; i < board->rows; i++)
		for (j = 0; j < board->cols; j++)
			if (board->cells[i][j] == EMPTY_CELL)
				return 0;
	return 1;
}

static int check_diagonal(const Board* board, int r, int c, int dr, int dc, char symbol)
{
	int lenght = 0;
	int min_side = (board->rows < board->cols) ? board->rows : board->cols;

	while (r >= 0 && r < board->rows && c >= 0 && c < board->cols) {
		if (board->cells[r][c] != symbol)
			return 0;
		lenght++;
		r += dr;
		c += dc;
	}
	return lenght >= min_side && lenght > 1;
}

int board_check_win(const Board* board, char symbol)
{
	int i, j, line;

	for (i = 0; i < board->rows; i++) {
		line = 1;
		for (j = 0; j < board->cols; j++)
			if (board->cells[i][j] != symbol) {
				line = 0;
				break;
			}
		if (line && board->cols > 1)
			return 1;
	}

	for (j = 0; j < board->cols; j++) {
		line = 1;
		for (i = 0;i < board->rows; i++)
			if (board->cells[i][j] != symbol) {
				line = 0;
				break;
			}
		if (line && board->rows > 1)
			return 1;
	}

	//if (check_diagonal(board, 0, 0, 1, 1, symbol))
	//	return 1;
	//if(check_diagonal(board, 0, board->cols - 1, -1, -1, symbol))
	//	return 1;
	//if (check_diagonal(board, board->rows - 1, 0, -1, 1, symbol))
	//	return 1;
	//if (check_diagonal(board, board->rows - 1, board->cols - 1, -1, -1, symbol))
	//	return 1;

	for (i = 0; i < board->rows; i++) {
		if (check_diagonal(board, i, 0, 1, 1, symbol))
			return 1;
		if (check_diagonal(board, i, board->cols - 1, 1, -1, symbol))
			return 1;
	}
	for (j = 0; j < board->cols; j++) {
		if (check_diagonal(board, 0, j, 1, 1, symbol))
			return 1;
		if (check_diagonal(board, 0, j, 1, -1, symbol))
			return 1;
	}

	return 0;
}