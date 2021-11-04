#include "func.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *file; char path[50]; char key;
	int width, height;
	Cell *cells;
	cells = initCells();
	if(argc == 2) tryopen(argv[1], &file);
	else{
		printf("Szeretné, ha az alapértelemezett fált olvasnám be? (I/n) \n (./maps/default.map)");
		key = getchar();
		if (key == 'i') {
		file = fopen("./maps/default.map", "r+");
		}else{
		tryopen(path, &file);
		}
	}
	readsize(&width, &height, file);
}

