
#include <stdio.h>
typedef struct BaseCell {
	// a cell elhagyásának költsége, illetve a cell távolsága a céltól
	// a speed az időköltség
	float speed, distance; 
	char symbol;
	int cost; // az eddigi út költsége, speedek összege
	struct cell *up, *down, *left, *right;
} base_cell;

typedef struct CityCell {
	float speed, distance;
	char symbol;
	char name[15];
	int cost;
	struct cell *up, *down, *left, *right;
} city_cell;

typedef struct ImpCell{
	char symbol;
} impassable_cell;

enum typelookup{BASECELL =1, CITYCELL = 2, IMPASSABLE = 3};

typedef struct cell{
	union{
		city_cell CC;
		base_cell BC;
		impassable_cell IC;
	};
	int type;
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
int cell_search(Cell *cells, Cell *cellmap);
int link_cells(Cell **map, int width, int height);