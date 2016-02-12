#include "tsplib.h"

#define MAX_SIZE 	600000
#define MAX_NEIGH	19

enum header_types {
 NAME,		
 TYPE,		
COMMENT,		
 DIMENSION,	
 DISPLAY,		
 EDGE		
};
int get_lien_type(FILE *ofp) {
	char curr_c = 'f';
	char title[20];
	int index = 0;
	int rtn;

	while (curr_c != ':' && curr_c != '\n' && curr_c != '\0') {

		curr_c = fgetc(ofp);

		if (curr_c >= 'A' && curr_c <= 'Z') {
			title[index] = curr_c;
			index++;
		}

	}
	title[index] = '\0';

	if (strcmp(title, "NAME") == 0) {
		rtn = NAME;
	} else if (strcmp(title, "TYPE") == 0) {
		rtn = TYPE;
	} else if (strcmp(title, "COMMENT") == 0) {
		rtn = COMMENT;
	} else if (strcmp(title, "DIMENSION") == 0) {
		rtn = DIMENSION;
	} else if (strcmp(title, "DISPLAY_DATA_TYPE") == 0) {
		rtn = DISPLAY;
	} else if (strcmp(title, "EDGE_WEIGHT_TYPE") == 0) {
		rtn = EDGE;
	} else {
		rtn = -1;
	}
	return rtn;
}

int get_desipt(FILE * ofp, char *line) {
	char curr_c = 'f';
	int index = 0;

	while (curr_c != '\n' && curr_c != '\0') {
		curr_c = fgetc(ofp);
		if (index == 0 && curr_c == ' ') {
		} else if ((curr_c != '\n' && curr_c != '\0')) {
			line[index] = curr_c;
			index++;
		}
	}

}

int input_cities(FILE * ofp, tspfile * file) {
	int numAdded = 0;
	char line[100];
	int i;
	fgets(line, sizeof(line), ofp);
	fgets(line, sizeof(line), ofp);
	while (feof(ofp) == 0) {
		int index;
		float tempx, tempy;
		fgets(line, sizeof(line), ofp);

		sscanf(line, "%d %f %f", &index, &tempx, &tempy);
		file->nodes[index - 1].id = index;
		file->nodes[index - 1].pos = index - 1;
		file->nodes[index - 1].y = tempy;
		file->nodes[index - 1].x = tempx;
		
		numAdded++;

	}

	return numAdded;
}

int parse_file(char * file_name, tspfile * file) {
	FILE *ofp;
	char line[200];
	int type;

	if ((ofp = fopen(file_name, "r")) == NULL) {
		printf("fail to open file\n");
		return -1;
	}
	while ((type = get_lien_type(ofp)) != -1) {
		memset((void*) line, 0, sizeof(line));
		get_desipt(ofp, line);
		switch (type) {
		case NAME:
			strcpy(file->name, line);
			break;
		case TYPE:
			strcpy(file->type, line);
			break;
		case COMMENT:
			strcpy(file->comment, line);
			break;
		case DIMENSION:
			file->dimension = atoi(line);
			file->nodes = malloc(file->dimension * sizeof(node));
			break;
		case EDGE:
			strcpy(file->edge_weight_type, line);
			break;
		case DISPLAY:
			strcpy(file->display_data_type, line);
			break;
		}
	}
	if (file->dimension < MAX_SIZE) {
		input_cities(ofp, file);
	} else {	
		return -1;
	}

	return 0;
}
