#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *file; char path[50] = "./maps/default.map"; char key, **map = NULL;
	Definition *defs;
	int width, height, cell_length = 4, FILE_ERROR = 0;
	Cell cells[cell_length], **cellmap;
	initCells(cells);
// Fájlbeolvasás kezdete
do{
	if(argc == 2) tryopen(argv[1], &file);
	else{
		printf("Szeretné, ha az alapértelemezett fált olvasnám be? (I/n) \n (./maps/default.map)");
		key = getchar(); // ide *elvileg* nem kell entert szűrni, mert ez lesz az első 
		if (key == 'i') {
		file = fopen(path, "r+");
		}else{
		tryopen(path, &file);
		}
	}
	readsize(&width, &height, file);
	readmap(&map, width, height, file);
	FILE_ERROR = parse_cities(file, defs);
	//fájlbeolvasás vége
}
while(!FILE_ERROR);

	convert_nulls(map, file, height, width);
	convert_tocells(map, height, width, cells, &cellmap);
}

