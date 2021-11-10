#include <stdio.h>
typedef struct BaseCell {
	// a cell elhagyásának költsége, illetve a cell távolsága a céltól
	// a speed az időköltség
	float speed, distance; 
	char symbol;
	int cost, visited; // az eddigi út költsége, speedek összege
} base_cell;

typedef struct CityCell {
	base_cell bc;
	char name[100];
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
	struct cell *up, *down, *left, *right;
} Cell;
typedef struct cellsortable{
	Cell *current;
	struct cellsortable *next, *prev;
} cell_sortable;
typedef struct point {int x, y;} Point;
typedef struct definition{Point P; char name[100]; struct definition *next;} Definition;
void tryopen(char *arg, FILE **f);
void enterpath(FILE **f, char *path);
void initCells(Cell *cells);
void readsize(int *width, int *height, FILE* file);
float babylonian(int square);
float square(float base);
void readmap(char ***map, int width, int height, FILE *);
void convert_nulls(char **map, FILE *, int height, int width);
int convert_tocells(char **map, int height, int width, Cell *cells, Cell ***cellmap);
int cell_search(Cell *cells, char c, Cell *cellmap);
cell_sortable *link_cells(Cell **map, int width, int height);
void cellneighbour(Cell ** map , int x, int y, int width, int height);
int parse_cities(FILE *file, Definition *defs);
int parse_defs(Definition *def, FILE *file);
Point parse_point(char *string);
int power10(int n);
int reverse_bubble(cell_sortable *unvis);
void swap (cell_sortable *cs);