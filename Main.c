#ifdef _MSC_VER
#define CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "input.h"

static int make_move(Board* board, int player_number, char symbol)
{
	int row, col;

	for (;;) {
		if (!read_move(board, player_number, symbol, &row, &col))
			return 0;
		if (board_place(board, row, col, symbol) == PLACE_OK)
			return 1;

		printf("Эта клетка уже занята, выберите другую.\n");
	}
}

static int play(Board* board, const char* symbols)
{
	int current = 0;

	for (;;) {
		board_print(board);

		if (!make_move(board, current + 1, symbols[current]))
			return -2;

		if (board_check_win(board, symbols[current]))
			return current;

		if (board_is_full(board))
			return -1;

		current = 1 - current;
	}
}

int main(void)
{
	Board board;
	char symbols[2];
	int rows, cols, result;

#ifdef _WIN32
	system("chcp 65001 > nul");
#endif

	printf("=== Крестики-Нолики ===\n");

	read_board_size(&rows, &cols);
	read_symbol(1, &symbols[0], '\0');
	read_symbol(2, &symbols[1], symbols[0]);

	if (!board_init(&board, rows, cols)) {
		printf("Не удалось выделить память под поле.\n");
		return 1;
	}

	result = play(&board, symbols);

	board_print(&board);
	if (result >= 0)
		printf("Победил игрок %d (%c)!\n", result + 1, symbols[result]);
	else if (result == -1)
		printf("Ничья: свободных клеток не осталось.\n");
	else
		printf("Игра прервана.\n");

	board_free(&board);
	return 0;
}