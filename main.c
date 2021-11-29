#include "func.h"
#include <stdio.h>
#include <stdlib.h>
//valahogy ez a megoldás működik az interaktivitással vívott küzdelemben....
#define INPUT_BUFF 50
int main(int argc, char *argv[]){
	FILE *file; char path[50] = "./maps/default.map"; char key, buffer[INPUT_BUFF], same_map_key, cancellation, **charmap = NULL;
	Definition *defs = calloc(1, sizeof(Definition));
	int width, height, cell_length = 4, FILE_ERROR = 0, first_run = 1, exiting = 0;
	Cell *cells = calloc(cell_length, sizeof(Cell)), **cellmap, *goal;
	cell_sortable *unvisited = 0;
	initCells(cells);
// Fájlbeolvasás kezdete
do{
	FILE_ERROR = 0;
	if(argc == 2) tryopen(argv[1], &file);
	else{
		printf("Szeretné, ha az alapértelemezett fált olvasnám be? (I/n) \n (./maps/default.map) ");
		key = getchar(); // ide *elvileg* nem kell entert szűrni, mert ez lesz az első 
		if (key == 'i') {
			file = fopen(path, "r");
		}else{
			printf("Kérem írja be a fájl elérési útját! ");
			fscanf(stdin, "%s", path);
			tryopen(path, &file);
		}
	} while (getchar() != '\n') {}
	if(readsize(&width, &height, file) != 2){
		FILE_ERROR = 1; continue;
	}
	readmap(&charmap, width, height, file);
	FILE_ERROR = parse_cities(file, &defs);

	//fájlbeolvasás vége
	}while(FILE_ERROR == 1);
	while(!exiting){
	if (first_run == 0) {
		while(1){
			printf("Szeretne ugyanezen a térképen egy másik utat keresni? i/n \n");
			fgets(buffer, sizeof(buffer), stdin);
			sscanf(buffer, "%c", &same_map_key);
			if (same_map_key) {
				if (same_map_key == 'n') {
					enterpath(&file);
					free_charmap(height, charmap);
					free_cellmap(height, cellmap);
					free_definitions(defs); defs = calloc(1, sizeof(Definition));
					cleanup_unvisited(unvisited);
					readsize(&width, &height, file);
					readmap(&charmap, width, height, file);
					parse_cities(file, &defs);
					list_cities(defs);
					break;
				}else if (same_map_key == 'i') {
					break;
				}else {
				printf("Kérem igennel vagy nemmel válaszoljon! \n");
				}
			}
		}
	}else {
	first_run = 0;
	}
	convert_nulls(charmap, file, height, width);
	convert_tocells(charmap, height, width, cells, &cellmap);
	name_cities(defs, cellmap);
	
	unvisited = link_cells(cellmap, width, height);
	list_cities(defs);
	switch (dijkstra(unvisited, (goal = ask_goal(height, width, cellmap, defs)))){
		case -1:
			printf("Sajnos a megadott két pont között nem létezik út. \n");
		break;
		case 1:
		print_routed_map(cellmap, goal, height, width);
		break;
	}
	do {
		printf("Szeretné leállítani a programot? i/n ");
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer, " %c%*s", &cancellation);
		switch (cancellation) {
		case 'i':
		exiting = 1;
		break;
		case 'n': break;
		default:
			printf("Kérem válaszoljon igennel, vagy nemmel! \n");
		}
	}while (!(cancellation != 'i' || cancellation != 'n'));
	}
}

