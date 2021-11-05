#include <stdio.h>
typedef struct Cell {
	float speed;
	char symbol;
	int cost;
} Cell;

void tryopen(char *arg, FILE **f);
void enterpath(FILE **f, char *path);
void initCells(Cell *cells);
void readsize(int *width, int *height, FILE* file);
float babylonian(int square);
float square(float base);
void readmap(char *map[], int width, int height, FILE *);