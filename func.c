#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // mivel a végtelen, mint érték, nem létezik c-ben, 
					//ezért a lehető legnagyobb érték megadásához kell ez a header
void tryopen(char *arg, FILE **f){
	char key = 'n';
	if (!(*f = fopen(arg, "r+"))){
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
	*f = fopen("./maps/default.map", "r+");
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

Cell *initCells(){ // lehetne egy kicsit optimalizálni, kevesebb változót használni, 
					//de magamat ismerve nem fogom tudni követni később
	Cell passable, inpassable, city, freeway;
	Cell *cells = malloc(sizeof(Cell)*4);
	inpassable.speed = 0;
	inpassable.symbol = '#';

	passable.speed = 3;
	passable.symbol = '.';
	passable.cost = INT_MAX;

	freeway.speed = 2;
	freeway.symbol = '=';
	freeway.cost = INT_MAX;

	city.speed = 2;
	city.symbol = ':';
	city.cost = INT_MAX;
	cells[0] = inpassable; 
	cells[1] = passable; 
	cells[2] = freeway; 
	cells[3] = city;
	return cells;
}

void readsize(int *width, int *height, FILE* file){
	fscanf(file, "%d %d", width, height); // első sor beolvasva, visszaadja a szélesség*magasságot
}

float babylonian(float square){ // Babilóniai módszerrel számolok négyzetgyököt
	float guess = square / 2,temp;
	do {
		temp = guess;
		guess = (guess + square/guess)/2;
	} while((temp - guess)*(temp - guess) > 0.00001);
	return guess;
}