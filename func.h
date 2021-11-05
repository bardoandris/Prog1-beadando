#include <bits/types/FILE.h>
#include <stdio.h>
typedef struct Cell {
	float speed, distance;
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
void convert_nulls(char **map, FILE *, int height, int width);
int convert_tocells(char **map, int height, int width, Cell *cells, Cell **cellmap);