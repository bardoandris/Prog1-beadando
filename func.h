#include <stdio.h>
typedef struct Cell {
	float speed;
	char symbol;
} Cell;

void tryopen(char *arg, FILE **f);
void enterpath(FILE **f);
Cell *initCells();
void readsize(int width, int height, FILE* file);