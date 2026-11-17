#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

#define LINE_SIZE 128

static int read_line(char* buffer, size_t size)
{
	size_t len;
	int ch;

	if (fgets(buffer, (int)size, stdin) == NULL)
		return 0;

	len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0';
	}
	else {
		while ((ch = getchar()) != '\n' && ch != EOF);
	}
	return 1;
}

void read_board_size(int* rows, int* cols)
{
	char line[LINE_SIZE];
	int n, m;
	char extra;

	for (;;) {
		printf("Введите размер поля N M (от %d до %d), либо Enter для %dx%d: ", MIN_SIZE, MAX_SIZE, DEFAULT_SIZE, DEFAULT_SIZE);

		if (!read_line(line, sizeof(line))) {
			*rows = DEFAULT_SIZE;
			*cols = DEFAULT_SIZE;
			return;
		}
		if (line[0] == '\0') {
			*rows = DEFAULT_SIZE;
			*cols = DEFAULT_SIZE;
			return;
		}
		if (sscanf(line, "%d %d %c", &n, &m, &extra) == 2
			&& n >= MIN_SIZE && n <= MAX_SIZE
			&& m >= MIN_SIZE && m <= MAX_SIZE) {
			*rows = n;
			*cols = m;
			return;
		}
		printf("Некорректный ввод. Пример: 4 5\n");
	}
}

void read_symbol(int player_number, char* symbol, char forbidden)
{
	char line[LINE_SIZE];

	for (;;) {
		printf("Игрок %d, выберите свой символ (один ASCII-символ, не пробел): ", player_number);

		if (!read_line(line, sizeof(line))) {
			*symbol = (player_number == 1) ? 'X' : 'O';
			return;
		}
		if (strlen(line) != 1 || line[0] < 33 || line[0] > 126) {
			printf("Нужен ровно один видимый ASCII-символ.\n");
			continue;
		}
		if (line[0] == forbidden) {
			printf("Этот символ уже занят другим игроком.\n");
			continue;
		}
		*symbol = line[0];
		return;
	}
}

int read_move(const Board* board, int player_number, char symbol, int* row, int* col)
{
	char line[LINE_SIZE];
	int r;
	char letter, extra;
	char c;

	for (;;) {
		printf("Ход игрока %d (%c). Введите строку и столбец (например, 2 B): ", player_number, symbol);

		if (!read_line(line, sizeof(line)))
			return 0;

		if (sscanf(line, "%d %c %c", &r, &letter, &extra) == 2){
			c = toupper((unsigned char)letter) - 'A';
			if (r >= 1 && r <= board->rows && c >= 0 && c < board->cols) {
				//&& r >= 1 && r <= board->rows
				//&& c >= 1 && c <= board->cols) {
				*row = r - 1;
				*col = c;
				return 1;
			}
		}
		printf("Некорректный ввод. Строка 1..%d, столбец A..%c, например: 2 B.\n", board->rows, 'A' + board->cols - 1);
	}
}