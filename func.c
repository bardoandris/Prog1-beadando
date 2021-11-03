#include "func.h"
#include <stdio.h>
#include <stdlib.h>
void tryopen(char *arg, FILE **f){
	char key = 'n';
	if (!(*f = fopen(arg, "r+"))){
		printf("Ez a fájl nem létezik!\n Szeretné, ha az alapértelmezetett nyitnám meg? (i/N)\n");
		printf("./maps/default.map ");
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
	char* path = malloc(sizeof(char) * 50);
	do {
		printf("Kérem adja meg a fájl elérését! \n");
		scanf("%s", path);
		*f = fopen(path, "r");
		if (!f) printf("Nem valós fájlt adott meg!");
	}while (!f);
}

Cell *initCells(){ // lehetne egy kicsit optimalizálni, kevesebb változót használni, csak lehet nem fogom tudni követni később
	Cell passable, inpassable, city, freeway; 
	Cell *cells = malloc(sizeof(Cell)*4);
	inpassable.speed = 0;
	inpassable.symbol = '#';

	passable.speed = 3;
	passable.symbol = '.';

	freeway.speed = 2;
	freeway.symbol = '=';

	city.speed = 2;
	city.symbol = ':';
	cells[0] = inpassable; 
	cells[1] = passable; 
	cells[2] = freeway; 
	cells[3] = city;
	return cells;
}

void readsize(int width, int height, FILE* file){
	fscanf(file, "%d %d", &width, &height); // első sor beolvasva, visszaadja a szélesség*magasságot
}