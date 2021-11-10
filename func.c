#include "func.h"
#include <stdio.h>
#include <stdlib.h>
void tryopen(char *arg, FILE **f){
	char key = 'n';
	if (!(*f = fopen(arg, "r+"))){
		printf("Ez a fájl nem létezik!\n Szeretné, ha az alapértelmezetett nyitnám meg? (i/N)\n");
		printf("./maps/default.map");
		do{ 
		key = getchar();
		
		} while (key == '\n');
		if (key == 'n') {
		enterpath(f, arg); 
		return; 
		}
	}
	*f = fopen("./maps/default.map", "r+");
}

void enterpath(FILE **f, char *path){
	do {
		printf("Kérem adja meg a fájl elérését! \n");
		scanf("%s", path);
		*f = fopen(path, "r");
		if (!f) printf("Nem valós fájlt adott meg!");
	}while (!f);
}

void initCells(Cell *cells){	// lehetne egy kicsit optimalizálni, kevesebb változót használni, 
					// de magamat ismerve nem fogom tudni követni később
	base_cell bc, hwc; impassable_cell ic; city_cell cc;
	bc.speed = 3;	bc.distance = -1;	bc.symbol = '.';	bc.visited = 0;
	hwc.speed = 2;	hwc.distance = -1;	hwc.symbol = '=';	hwc.visited = 0;
	cc.bc.speed = 2;	cc.bc.distance = -1;	cc.bc.symbol = ':';	cc.bc.visited = 0;
	cells[0].BC = bc; cells[0].type = BASECELL;
	cells[1].BC = hwc; cells[1].type = BASECELL;
	cells[2].CC = cc; cells[2].type = CITYCELL;
	cells[3].IC = ic; cells[3].type = IMPASSABLE;
}
 // első sor beolvasva, visszaadja a szélesség*magasságot
void readsize(int *width, int *height, FILE* file){
	fscanf(file, "%d %d", width, height);
}
// Babilóniai módszerrel számolok négyzetgyököt
float babylonian(int S){ 
	float guess;
	switch (S) {
	case 1:
		guess = 1;
	break;
	case 2:
		guess = 1.5;
	break;
	default:
	guess = (float)S / 2;
	break;
	}
	do {
		guess = (guess + S/guess)/2;
	} while(square(S - guess * guess) > 0.000001);
	return guess;
}
float square(float base){
	return base * base;
}
//VIGYÁZZ!! a beolvasott tömböt t[y][x] formában kell indexelni(előbb vertikálisan,majd horizontásisan)
void readmap(char ***map, int width, int height, FILE* file){
	*map = (char**)calloc(height, sizeof(char**));
	for (int x = 0; x < height; x++) {
		(*map)[x] = (char*)calloc(width, sizeof(char));
		fscanf(file, "%s", *map[x]);
	}
}
void convert_nulls(char **map, FILE * file, int height, int width){
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width - 1; y++) {
			if (map[x][y] == 0) {
				map[x][y] = '#';
			}
		}
	} 
}

int convert_tocells(char **map, int height, int width, Cell *cells, Cell ***cellmap){
	*cellmap = (Cell**)calloc(height, sizeof(Cell*));
	for (int x = 0; x < height; x++) {
		(*cellmap)[x] = (Cell*)calloc(width, sizeof(Cell));
		for (int y = 0; y < width; y++) {
		cell_search(cells, map[x][y], &(*cellmap)[x][y]);
		}
	}
}

int cell_search(Cell *cells, char c, Cell *cellmap){
	if ((uint)c > 127){
		// TODO HIBAKEZELÉS HA NEM ASCII KÓD (nem 0xxxxxxx)
	}
	switch (cellmap->type) {
	case IMPASSABLE:
		for (int x = 0; x < 4; x++) {
			if (cells[x].type == IMPASSABLE) {
			*cellmap = cells[x];
			return 0;
			}
		}
	case BASECELL:
		for (int x = 0; x < 4; x++) {
			if (cells[x].BC.symbol == c) {
				*cellmap = cells[x];
				return 0;
			}
		}
	case CITYCELL:
		break;
	}
	
	return 0;
}
//összeköti a celleket a szomszédjukkal, és visszaadja a meg nem látogatott cellek listáját
cell_sortable *link_cells(Cell **map, int width, int height){
	cell_sortable *first,*iter = calloc(1, sizeof(cell_sortable));
	iter = calloc(1, sizeof(cell_sortable));
	first = iter;
	for(int x = 0; x < height; x++){
		for (int y; y < width; y++) {
			if (!(map[x][y].type == IMPASSABLE)) {
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
int parse_cities(FILE *file, Definition *defs){
	defs = (Definition*)calloc(sizeof(Definition), 1);
	parse_defs(defs,file);
	defs->next = calloc(1, sizeof(Definition));
	
}

int parse_defs(Definition *def, FILE *file){
	int breaking = 0, x = 0, y = 0, i = 0, size, num_size = 25;
	char c, *string = calloc(sizeof (char), num_size);
	while ((c = getc(file)) != '\n'){
		switch (c) {
		case EOF:
		return 2;
		case '(':
		breaking = 1; // sajnos egy break; nem működne itt
		}
	};

	while ((c = getc(file))  >= '0' && c <= '9' ){
		if (i < num_size - 1) string[i] = c;
		else return 1;
		i++;
	};
	size = i;
	if (c != EOF && c != '\n'){
		for (; i >= 0; i--) {
			x += string[i] * (size - i);
		}
	}else return 1;
	def->P.y = x - 2; //egy sor lecsúszik, egy a pointer indexelés miatt
	while ((c = getc(file))  >= '0' && c <= '9' ){
		if (i < num_size - 1) string[i] = c;
		else return 1;
		i++;
	};
	size = i;
	if (c != EOF && c != '\n'){
		for (; i >= 0; i--) {
			y += string[i] * (size - i);
		}
	}else return 1;
	def->P.y = y; //egyet levonunk a 
	fgets(def->name, 100, file);
	return 0;
}
int reverse_bubble(cell_sortable *cs){
	int iteration_count, sorted;
	cell_sortable *iterator = cs;
	do{
		sorted = 1;
		if (iterator->current->type == CITYCELL && iterator->next != NULL) {
			if	(iterator->current->CC.bc.cost < 
				iterator->next->current->CC.bc.cost){
					swap(iterator);
					sorted = 0;
				}
		}else {
		if (iterator->next != NULL) {
			if (iterator->current->BC.cost < iterator->next) {
			statements
			}
		}
		}
	}while(!sorted);

}
// x(n) x(n+1) == cs x(n+2) x(n+3) --> x(n) x(n+2) x(n+1) == cs x(n+3) 
void swap(cell_sortable *cs){
	cell_sortable *x0, *x1, *x2, *x3;
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
void cellneighbour(Cell **map, int x, int y, int width, int height){
	int has_up = 1, has_down = 1, has_right = 1, has_left = 1;
	if (x == 0) {
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