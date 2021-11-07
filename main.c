#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *file; char path[50] = "./maps/default.map"; char key, **map = NULL;
	int width, height, cell_length = 4;
	Cell cells[cell_length], **cellmap;
	initCells(cells);
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
	readmap(map, width, height, file);
	convert_nulls(map, file, height, width);
	convert_tocells(map, height, width, cells, cellmap);
}

