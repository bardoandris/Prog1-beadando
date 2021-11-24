#include "func.h"
#include <stdio.h>
#include <stdlib.h>
int City_N_Length = 100;

void tryopen(char *arg, FILE **f){
	char key = 'n';
	if (!(*f = fopen(arg, "r"))){
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
}

void enterpath(FILE **f, char *path){
	do {
		printf("Kérem adja meg a fájl elérését! \n");
		scanf("%s", path);
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
	/*cells[0] = bc; cells[0].type = BASECELL;
	cells[1] = hwc; cells[1].type = BASECELL;
	cells[2] = cc; cells[2].type = CITYCELL;
	cells[3] = ic; cells[3].type = IMPASSABLE; */
}
 // első sor beolvasva, visszaadja a szélesség*magasságot
int readsize(int *width, int *height, FILE* file){
	char size[20];
	fgets(size, 20, file);
	return sscanf(size, "%d %d\n", width, height);
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
//VIGYÁZZ!! a beolvasott tömböt t[x][y]-ban lehet indexelni, ahol x a sorszám, y az oszlopszám
void readmap(char ***map, int width, int height, FILE* file){
	*map = (char**)calloc(height, sizeof(char*));
	for (int x = 0; x < height; x++) {
		(*map)[x] = (char*)calloc(width+1, sizeof(char)); // plusz egy szélesség, hogy n szélesség mellett n+1 legyen a lezáró NULL bájt
		fgets((*map)[x], width+1, file);
		printf("%d\t%s", x , (*map)[x]);
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
	/*char c;
	while ((c = getc(file)) != '\n' && breaking != 1){
		switch (c) {
		case EOF:
		return 2;
		case '(':
		breaking = 1; // sajnos egy break; nem működne itt
		}
	}
	if ((deb = fscanf(file, "%d%*c%d%*c%s", &def->P.x, &def->P.y, def->name))) { // pl (x;y)Budapest
		return 0;
	}
	return 1;*/
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
cell_sortable *calculate_distance(int count, cell_sortable *cs){
	cell_sortable *iterator = cs, *smallest;
	int dist_min;
	for (; iterator->next != 0; iterator= iterator->next); // lista végére lépünk
	for ( int i = count; i > 0; i--) {
		iterator->current->distance = babylonian(square(iterator->current->pos.x) + square(iterator->current->pos.y));
	}
	for (; iterator->next != 0; iterator= iterator->next);
	dist_min = iterator->current->distance;
	smallest = iterator;
	for (int i = count; i > 0; i --) {
		iterator = iterator->prev;
		if (iterator->current->distance < dist_min ) {
			dist_min = iterator->current->distance;
			smallest = iterator;
		}
	}
	return smallest;
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
// ez fogja megadni azt a cellát, amire végre kell hajtani a "látogatást"
cell_sortable *minimum_distance(cell_sortable **unvisited){
	int i;
	i = determine(*unvisited);
	if (i == -1) {
	return 0;
	}
	return 1;
}
int visit(cell_sortable *vis){
	Cell 
		*up = vis->current->up,
		*down = vis->current->down,
		*left = vis->current->left,
		*right = vis->current->left;
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
		if(visit(bubble(&unvisited))){
			print_route(goal);
			return 0;
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
			if (search_city(input, City_N_Length, map, width, height).x < 0){
				printf("A megadott helyen nem található érvényes cella, kérem próbálja újra\n");
				printf("Érvényes Városnevek: \n");
				list_cities(defs);
				continue;
			}
		}
	}
	
	while(1){
		printf("Hova menjen a GPS? (Pozíció \"x:y\" vagy \"Városnév\")\n");
		fgets(input, City_N_Length, stdin);
		if (sscanf(input, " %d:%d", &finish.x, &finish.y) == 2) {
			if (finish.x >= 0 && finish.x < width && finish.y >= 0 && finish.y < width
				&& map[finish.x][finish.y].type != IMPASSABLE && !compare_position(map[start.x][start.y].pos, map[finish.x][finish.y].pos)) {
					break;
			}
		}else {
			if ((finish = search_city(input, City_N_Length, map, width, height)).x < 0){
				printf("A megadott helyen nem található érvényes cella, kérem próbálja újra\n");
				printf("Érvényes Városnevek: \n");
				list_cities(defs);
				continue;
			}
		}
	}
	init_goal(&map[finish.x][finish.y], &map[start.x][start.y]);
	return &map[finish.x][finish.y];
}
Point search_city(char *input, int length, Cell **map, int width, int height){
	Cell selection; Point P; int i, strlen = 0;
	sscanf(input, "%s\n", input); // enter kiszűrése
	for (; strlen < length && input[strlen] != 0; strlen++)
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				if (map[h][w].type == CITYCELL) {
					selection = map[h][w];
					for (i = 0; i < length && selection.name[i] != 0
						&& selection.name[i] == input[i]; i++) {}
					if (i == strlen) {
						P.x = h; P.y = w;
						return P;
					}
				}
			}
		}
	P.x = -1; return P;
}
void list_cities(Definition *defs){
	for (; defs->next; defs = defs->next) {
		printf("%s: %d ; %d \n", defs->name, defs->P.x, defs->P.y);
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
	while (route->from_where != 0) {
	printf("(%d:%d) ", route->pos.x, route->pos.y);
	if (route->type == CITYCELL) {
		printf("%s\n", route->name);
	}else {
	printf("\n");
	}
	route = route->from_where;
	}
	
}
// ellenőrzi, van-e olyan Cell cím, ami nem elérhető
void nullcell(cell_sortable *unvis){
	
	for (; unvis->next != 0; unvis = unvis->next) {
		printf("{%d,%d} ", unvis->current->pos.x, unvis->current->pos.y);
	}
	printf("\n");
}