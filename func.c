#include "func.h"
#include <stdio.h>
#include <stdlib.h>
int City_N_Length = 100;
char leftarrow = '<', rightarrow = '>',
uparrow = '^', downarrow = 'v';

void tryopen(char *arg, FILE **f){
	char key = 'n';
	if (!(*f = fopen(arg, "r"))){
		printf("Ez a fájl nem létezik!\n Szeretné, ha az alapértelmezetett nyitnám meg? (i/N)\n");
		printf("./maps/default.map");
		do{ 
		key = getchar();
		
		} while (key == '\n');
		if (key == 'n') {
		enterpath(f);
		return;
		}
	}
}

void enterpath(FILE **f){
	char path[50];
	do {
		printf("Kérem adja meg a fájl elérését! \n");
		scanf("%49s", path); //az 50. hely az egy NULL
		*f = fopen(path, "r");
		if (!*f) printf("Nem valós fájlt adott meg!");
	}while (!*f);
}

void initCells(Cell *cells){	// lehetne egy kicsit optimalizálni, kevesebb változót használni, 
					// de magamat ismerve nem fogom tudni követni később
	Cell *bc = &cells[0], *hwc = &cells[1], *cc = &cells[2], *ic = &cells[3]; int MAX = maxint();
	bc->speed = 3;	bc->cost = MAX;	bc->symbol = '.';	bc->name = 0;	bc->type = BASECELL;		//bc.visited = 0;
	hwc->speed = 2;	hwc->cost = MAX;	hwc->symbol = '=';	hwc->name = 0;	hwc->type = BASECELL;	//hwc.visited = 0;
	cc->speed = 2;	cc->cost = MAX;	cc->symbol = ':';	cc->name = 0; cc->type = CITYCELL;			//cc.visited = 0;
	ic->symbol = '#'; ic->type = IMPASSABLE;
}
 // első sor beolvasva, visszaadja a szélesség*magasságot
int readsize(int *width, int *height, FILE* file){
	char size[20];
	fgets(size, 20, file);
	return sscanf(size, "%d %d\n", width, height);
}
//VIGYÁZZ!! a beolvasott tömböt t[x][y]-ban lehet indexelni, ahol x a sorszám, y az oszlopszám
void readmap(char ***map, int width, int height, FILE* file){
	*map = (char**)calloc(height, sizeof(char*));
	for (int x = 0; x < height; x++) {
		(*map)[x] = (char*)calloc(width+1, sizeof(char)); // plusz egy szélesség, hogy n szélesség mellett n+1 legyen a lezáró NULL bájt
		fgets((*map)[x], width+1, file);
		printf("%d\t%s", x, (*map)[x]);
	}
}
void convert_nulls(char **map, FILE * file, int height, int width){
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			if (map[x][y] == 0 || map[x][y] == '\n') { //lehet, hogy egy sor nem "width" széles, és a sor közepén van egy enter, amit NULL-nak veszünk
				map[x][y] = '#';
			}
		}
	} 
}
void print_map(int height, char** map){
	for (int h = 0; h < height; h++) {
		printf("%s", map[h]);
	}
}

int convert_tocells(char **map, int height, int width, Cell *cells, Cell ***cellmap){
	*cellmap = (Cell**)calloc(height, sizeof(Cell*));
	for (int x = 0; x < height; x++) {
		(*cellmap)[x] = (Cell*)calloc(width, sizeof(Cell));
		for (int y = 0; y < width; y++) {
		if (cell_search(cells, map[x][y], &(*cellmap)[x][y])){
			return 1;
		}
		(*cellmap)[x][y].pos.x = x;
		(*cellmap)[x][y].pos.y = y;
		}
	}
	return 0;
}

int cell_search(Cell *cells_list, char c, Cell *cellmap){
	if ((uint)c > 127){
	return 1;
	}
	for (int x = 0; x < 4; x++) {
		if (cells_list[x].symbol == c) {
			*cellmap = cells_list[x];
		}
	}
	
	return 0;
}
//összeköti a celleket a szomszédjukkal, és visszaadja a meg nem látogatott cellek listáját
cell_sortable *link_cells(Cell **map, int width, int height){
	cell_sortable *first,*iter = calloc(1, sizeof(cell_sortable));
	first = iter;
	for(int x = 0; x < height; x++){
		for (int y = 0; y < width; y++) {
			if (map[x][y].type != IMPASSABLE) {
				cellneighbour(map, x, y, width, height);
				iter->current = &map[x][y];
				iter->next = calloc(1, sizeof(cell_sortable));
				iter->next->prev = iter;
				iter = iter->next;
			}
		}
	}
	iter->prev->next = 0;
	free(iter);
	return first;
}
int parse_cities(FILE *file, Definition **defs){
	Definition *prev , *iterator = *defs;
	iterator->name = calloc(City_N_Length, sizeof(char));
	for (int x = 0; x != 1; iterator = iterator->next ) {
		prev = iterator;
		iterator->next = calloc(1, sizeof(Definition));
		iterator->name = calloc(City_N_Length, sizeof(char));
		x = parse_defs(iterator, file);
		
	}
	free(iterator->name);
	free(iterator);
	prev->next = 0;
	return 0;
}

int parse_defs(Definition *def, FILE *file){
	char temp[City_N_Length];
	if	(fgets(temp, City_N_Length, file)){
		if(sscanf(temp,"(%d;%d)%s", &def->P.x, &def->P.y, def->name) == 3){
			return 0;
		}
	}
	return 1;

}
cell_sortable *bubble(cell_sortable **unvis){
	int sorted;
	cell_sortable *iterator = *unvis, *rewind;
	do{
		sorted = 1;
		while (1) {
			if (iterator->next != 0) {
				if (iterator->current->cost < iterator->next->current->cost) {
				swap(iterator);
				sorted = 0;
				continue; //ha megtörtént a csere, akkor a jelenlegi elemet akarjuk tobább "buborékoltatni"
				}
			}else {
			break;
			}
			iterator = iterator->next;
		}
		
	}while(!sorted);
	rewind = iterator;
	for (; rewind->prev != 0; rewind = rewind->prev) {}
	*unvis = rewind;
	for (; iterator->next != 0; iterator = iterator->next) {}
	if (iterator->current->cost == maxint()) {
		return 0;
	}
	return iterator;
}
//visszaadja, hogy hány darab van a legkisebb elemből
//ha több mint egy, akkor el kell végezni a heurisztikus számolást (count-szor)
int determine(cell_sortable *cs){
	cell_sortable *iterator = cs; int min, count = 1;
	for (;iterator->next != 0; iterator=iterator->next); // ellépteti az iteratort a lista végére
	min = iterator->current->cost;
		if (min == maxint()) {
	return -1;
	}
	for ( iterator = iterator-> prev; iterator->prev != 0; iterator = iterator->prev) {
		if (iterator->current->cost == min) {
			count++;
		}
	}
	return count;
}
// x(n) x(n+1) == cs x(n+2) x(n+3) --> x(n) x(n+2) x(n+1) == cs x(n+3) 
void swap(cell_sortable *cs){
	cell_sortable *x0, *x1, *x2, *x3;	//a bubble algoritmusunkból garanciánk van,
										//hogy van a "cs" után elem, így legrosszabb esetben (x3 == 0)
	x0 = cs->prev; x1 = cs; x2 = cs->next; x3 = cs->next->next;
	if(x0 != 0){
		x0->next = x2;
	}
	x1->prev = x2; x1->next = x3;
	x2->prev = x0; x2->next = x1;
	if (x3 !=0) {
	x3->prev = x1;
	}
}
int maxint(){
	int size = 8 * sizeof(int);
	return 1 << (size - 2);
}
void cellneighbour(Cell **map, int x, int y, int width, int height){
	int has_up = 1, has_down = 1, has_right = 1, has_left = 1;
	if (x == 0) { // maximum két oldalon lehet cella, azok sem két ellenkezők, különben 1 széles/magas a térkép
		has_up = 0;
	}else if (x == height - 1) {
		has_down = 0;
	}
	if (y == 0) {
		has_left = 0;
	}else if (y == width - 1) {
		has_right = 0;
	}
	//fent
	if (has_up && map[x-1][y].type != IMPASSABLE) {
	map[x][y].up = &map[x-1][y];
	}
	//lenn
	if (has_down && map[x+1][y].type != IMPASSABLE) {
	map[x][y].down = &map[x+1][y];
	}
	//bal
	if (has_left && map[x][y-1].type != IMPASSABLE) {
	map[x][y].left = &map[x][y-1];
	}
	//jobb
	if (has_right && map[x][y+1].type != IMPASSABLE) {
	map[x][y].right = &map[x][y+1];
	}
}
/*
	-1: nincs érvényes útvonal
	0: futás alatt
	1: megvan a goal
*/
int visit(cell_sortable *vis){
	if (vis == 0) {
	return -1;
	}
	Cell 
		*up = vis->current->up,
		*down = vis->current->down,
		*left = vis->current->left,
		*right = vis->current->right;
	if	(up != 0 &&
		up->cost > vis->current->cost + vis->current->speed) {

		up->cost = vis->current->cost + vis->current->speed;
		up->from_where = vis->current;
		if (up->goal == 1) {
			return 1;
		}
	}
	if	(down != 0 &&
		down->cost > vis->current->cost + vis->current->speed) {

		down->cost = vis->current->cost + vis->current->speed;
		down->from_where = vis->current;
		if (down->goal == 1) {
			return 1;
		}
	}
	if	(left != 0 &&
		left->cost > vis->current->cost + vis->current->speed) {

		left->cost = vis->current->cost + vis->current->speed;
		left->from_where = vis->current;
		if (left->goal == 1) {
			return 1;
		}
	}
	if	(right != 0 &&
		right->cost > vis->current->cost + vis->current->speed) {

		right->cost = vis->current->cost + vis->current->speed;
		right->from_where = vis->current;
		if (right->goal == 1) {
			return 1;
		}
	}
	eliminate(vis);
	return 0;
}
int dijkstra(cell_sortable *unvisited, Cell *goal){
#ifdef DEBUG
	int x = 0;
#endif
	while (1) {
		switch (visit(bubble(&unvisited))) {
			case -1:
				return -1;
			break;

			case 1:
			mark_route(goal);
				return 1;
		}
		#ifdef DEBUG
		nullcell(unvisited);
		printf("%d", x++);
		#endif
	}
}
// x0, x1, x2 ==> x0, x2
void eliminate(cell_sortable *vis){
	cell_sortable *x0 = vis->prev, *x1 = vis, *x2 = vis->next;
	if (x0 == 0 && x2 != 0) {
		x2->prev = 0;
	}
	if (x2 == 0 && x0 != 0) {
		x0->next = 0;
	}
	if (x0 != 0 && x2 != 0) {
		x0->next = x2;
		x2->prev = x0;
	}
	free(vis);
}
void name_cities(Definition *defs, Cell **map){
	for (; defs->next != 0; defs = defs->next) {
		if (map[defs->P.x][defs->P.y].type == CITYCELL){
			map[defs->P.x][defs->P.y].name = defs->name;
	}else{
			printf("WARN: NEM VÁROSBA PRÓBÁLT NÉVÍRÁS\n HIBA: ");
			printf("(%d;%d): %s\t", defs->P.x, defs->P.y, defs->name);
			printf("NEM VÁROS\n");
		}
		}
	if (defs->next != 0) {
	printf("WARN: NEM MINDEN NÉV MEZŐ VAN FELHASZNÁLVA");
	}
}
void init_goal(Cell *gl, Cell *start){
	gl->goal = 1;
	start->cost = 0;

}
Cell *ask_goal(int height, int width, Cell** map, Definition* defs){
	char input[City_N_Length];
	int x = 0, valid = 0; Point start, finish;
	//Horizontális feldolgozás
	while(1){
		printf("Hol kezdjen a GPS? (Pozíció \"x:y\" vagy \"Városnév\")\n");
		fgets(input, City_N_Length, stdin);
		if (sscanf(input, " %d:%d", &start.x, &start.y) == 2) {
			if (start.x >= 0 && start.x < width && start.y >= 0 && start.y < width
				&& map[start.x][start.y].type != IMPASSABLE) {
				break;
			}
		}else {
			if ((start = search_city(input, City_N_Length, map, width, height, defs)).x < 0){
				printf("A megadott helyen nem található érvényes cella, kérem próbálja újra\n");
				printf("Érvényes Városnevek: \n");
				list_cities(defs);
				continue;
			}else {
				break;
			}
		}
	}
	
	while(1){
		printf("Hova menjen a GPS? (Pozíció \"x:y\" vagy \"Városnév\")\n");
		fgets(input, City_N_Length, stdin);
		if (sscanf(input, " %d:%d", &finish.x, &finish.y) == 2) {
			if (finish.x >= 0 && finish.x < width && finish.y >= 0 && finish.y < width
				&& map[finish.x][finish.y].type != IMPASSABLE ) {
					break;
			}
		}else {
			if (((finish = search_city(input, City_N_Length, map, width, height, defs))).x < 0){
				printf("A megadott helyen nem található érvényes cella, kérem próbálja újra\n");
				printf("Érvényes Városnevek: \n");
				list_cities(defs);
				continue;
			}else if (start.x != finish.y && start.y != finish.y){
			break;
			}
		}
		
	}
	init_goal(&map[finish.x][finish.y], &map[start.x][start.y]);
	return &map[finish.x][finish.y];
}
Point search_city(char *input, int length, Cell **map, int width, int height, Definition *defs){
	Cell selection; Point P; int match = 0, strlen = 0; P.x = 0;
	sscanf(input, "%s ", input); // enter kiszűrése
	for (; defs != 0; defs = defs->next) {
	if(comparestrings(input, defs->name) == 0){
		return defs->P;
	}
	}
	return P;

}
void list_cities(Definition *defs){
	for (; defs->next; defs = defs->next) {
		printf("%s: (%d;%d) \n", defs->name, defs->P.x, defs->P.y);
	}
}
int compare_position(Point A, Point B){
	int deltaX, deltaY;
	deltaX = A.x - B.x; deltaY = A.y - B.y;
	if (deltaX == 0 && deltaY == 0) {
		return 1;
	}
	return 0;
}
void print_route(Cell *goal){
	Cell *route = goal;
	while (route != 0) {
		printf("(%d:%d) ", route->pos.x, route->pos.y);
		if (route->type == CITYCELL) {
			printf("%s\n", route->name);
		}else {
		printf("\n");
		}
		route = route->from_where;
	}

}
void print_routed_map(Cell **map, Cell *goal, int height, int width){
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			switch (map[h][w].symbol) {
				case '#':
					printf("#");
					break;
				default:
					if (map[h][w].goal == 1) {
						printf("C");
					}else if (map[h][w].cost == 0 && map[h][w].type != IMPASSABLE) {
						printf("S");
					}else if(map[h][w].is_route == 1){
						printf("%c",decide_direction(&map[h][w]));
					}else {
					printf(" ");
					}
				break;
			}
		}
		printf("\n");
	}

}
char decide_direction(Cell *cell){
	if (cell->up && cell->up->is_route && cell->up->from_where == cell) {
		return '^';
	}
	if (cell->down && cell->down->is_route && cell->down->from_where == cell) {
		return 'v';
	}
	if (cell->left && cell->left->is_route && cell->left->from_where == cell) {
		return '<';
	}
	if (cell->right && cell->right->is_route && cell->right->from_where == cell) {
		return '>';
	}
	return ' ';
}
int comparestrings(char *key, char *value){
	int x = 0;
	for (x = 0; key[x] != '\0' && value[x] != '\0'; x++) {
		if (key[x] != value[x]) {
			return 1; // különböznek a stringek
		}
	}
	if (value[x] == '\0' && key[x] == '\0'){
		return 0;
	}else {
		return 1;
	}

}
void free_charmap(int height, char **map){
	for (int h = 0; h < height; h++) {
		free(map[h]);
	}
	free(map);
}
void free_cellmap(int height, Cell **map){
	for (int h = 0; h < height; h++) {
		free(map[h]);
	}
	free(map);
}
void free_definitions(Definition *defs){
	Definition *next_def;
	for (; defs != 0; defs = next_def) {
		next_def = defs->next;
		free(defs);
	}
}
void cleanup_unvisited(cell_sortable *unvis){
	cell_sortable *next_cs;
	for (; unvis != 0; unvis = next_cs) {
		next_cs = unvis->next;
		free(unvis);
	}
}
void mark_route(Cell *goal){
	for (; goal->from_where != 0; goal = goal->from_where) {
		goal->is_route = 1;
	}
}
//DEBUG TERRITORY
// ellenőrzi, van-e olyan Cell cím, ami nem elérhető
void nullcell(cell_sortable *unvis){
	
	for (; unvis->next != 0; unvis = unvis->next) {
		printf("{%d,%d} ", unvis->current->pos.x, unvis->current->pos.y);
	}
	printf("\n");
}

void printcells(Cell **map, int height, int width){
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			switch (map[x][y].type) {
			case IMPASSABLE:
				printf("#");
				break;
			case BASECELL:
				printf(".");
				break;
			case CITYCELL:
			printf(":");
			break;
			}
		}
			printf("\n");
	}
}