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
	bc.speed = 3;	bc.distance = -1;	bc.symbol = '.';
	hwc.speed = 2;	hwc.distance = -1;	hwc.symbol = '=';
	cc.speed = 2;	cc.distance = -1;	cc.symbol = ':';
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
void readmap(char **map, int width, int height, FILE* file){
	map = (char**)calloc(height, sizeof(char*));
	for (int x = 0; x < height; x++) {
		map[x] = (char*)calloc(width, sizeof(char));
		fscanf(file, "%s", map[x]);
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

int convert_tocells(char **map, int height, int width, Cell *cells, Cell **cellmap){
	cellmap = calloc(height, sizeof(Cell*));
	for (int x = 0; x < height; x++) {
		cellmap[x] = calloc(width, sizeof(Cell));
		for (int y = 0; y < width; y++) {
		cell_search(cells, width, char c, Cell *cellmap)
		}
	}
}

int cell_search(Cell *cells, int length, char c, Cell *cellmap){
	for (int x = 0; x < length; x++) {
	if (cells[x].symbol == c){
		*cellmap = cells[x];
		return 1;
	}
	}
	return 0;
}
int link_cells(Cell **map, int width, int height){

}
void reversebubble(){

}
