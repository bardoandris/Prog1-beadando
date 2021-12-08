#include <stdio.h>
// 0 az irány hiányát jelöli!
enum arrows{UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4}; 
enum typelookup{BASECELL =0, CITYCELL = 1, IMPASSABLE = 2};


typedef struct point {int x, y;} Point;
typedef struct cell{
	float speed;
	char symbol;
	int cost, type, goal, direction, is_route;
	Point pos;
	char *name;
	struct cell *up, *down, *left, *right, *from_where;
} Cell;
typedef struct cellsortable{
	Cell *current;
	struct cellsortable *next, *prev;
} cell_sortable;
typedef struct definition{Point P; char *name; struct definition *next;} Definition;
void tryopen(char *arg, FILE **f);
void enterpath(FILE **f);
void initCells(Cell *cells);
int readsize(int *width, int *height, FILE* file);
void readmap(char ***map, int width, int height, FILE *);
void convert_nulls(char **map, FILE *, int height, int width);
int convert_tocells(char **map, int height, int width, Cell *cells, Cell ***cellmap);
int cell_search(Cell *cells, char c, Cell *cellmap);
cell_sortable *link_cells(Cell **map, int width, int height);
void cellneighbour(Cell ** map , int x, int y, int width, int height);
int parse_cities(FILE *file, Definition **defs);
int parse_defs(Definition *def, FILE *file);
Point parse_point(char *string);
int determine(cell_sortable *cs);
int maxint();
int power10(int n);
cell_sortable *bubble(cell_sortable **unvis);
void swap (cell_sortable *cs);
cell_sortable *minimum_distance(cell_sortable **unvisited);
int dijkstra(cell_sortable **unvisited, Cell *goal);
int visit(cell_sortable *vis);
void eliminate(cell_sortable *vis);
void name_cities(Definition *defs, Cell **map);
int ask_function();
Cell *ask_goal(int height, int width, Cell **map, Definition *defs);
void init_goal(Cell *goal, Cell *start);
void list_cities(Definition *defs);
int compare_position(Point A, Point B);
Point parse_coords(char *string, int length);
Point search_city(char* input, int length, Cell **map, int width, int height, Definition *defs);
void print_route(Cell *goal);
int comparestrings(char *key, char *value);
char decide_direction(Cell *cell);
void print_routed_map(Cell **map, Cell *goal, int height, int width);
void mark_route(Cell *goal);
void free_charmap(int height, char **map);
void free_cellmap(int height, Cell **map);
void free_definitions(Definition *defs);
void cleanup_unvisited(cell_sortable *unvis);
//Szigorúan debug jelleggel alkamazott függvények
void nullcell(cell_sortable *unvis);
void printcells(Cell **map, int height, int width);