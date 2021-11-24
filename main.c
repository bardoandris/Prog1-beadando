#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *file; char path[50] = "./maps/default.map"; char key, **charmap = NULL;
	Definition *defs = calloc(1, sizeof(Definition));
	int width, height, cell_length = 4, FILE_ERROR = 0, cancellation = 0;
	Cell *cells = calloc(cell_length, sizeof(Cell)), **cellmap, *goal;
	cell_sortable *unvisited = 0;
	initCells(cells);
// Fájlbeolvasás kezdete
do{
	FILE_ERROR = 0;
	if(argc == 2) tryopen(argv[1], &file);
	else{
		printf("Szeretné, ha az alapértelemezett fált olvasnám be? (I/n) \n (./maps/default.map)");
		key = getchar(); // ide *elvileg* nem kell entert szűrni, mert ez lesz az első 
		if (key == 'i') {
			file = fopen(path, "r");
		}else{
			printf("Kérem írja be a fájl elérési útját!");
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

	convert_nulls(charmap, file, height, width);
	convert_tocells(charmap, height, width, cells, &cellmap);
	name_cities(defs, cellmap);
	
	//while(!cancellation){
	unvisited = link_cells(cellmap, width, height);
	
	dijkstra(unvisited, (goal = ask_goal(height, width, cellmap, defs)));
	//}
}

