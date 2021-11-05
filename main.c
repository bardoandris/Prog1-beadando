#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *file; char path[50] = "./maps/default.map"; char key, **map = NULL;
	int width, height;
	Cell cells[4];
	initCells(cells);
	if(argc == 2) tryopen(argv[1], &file);
	else{
		printf("Szeretné, ha az alapértelemezett fált olvasnám be? (I/n) \n (./maps/default.map)");
		key = getchar();
		if (key == 'i') {
		file = fopen(path, "r+");
		}else{
		tryopen(path, &file);
		}
	}
	readsize(&width, &height, file);
	readmap(map, width, height, file);
}

