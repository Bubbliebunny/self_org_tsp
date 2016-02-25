#include "tsplib.h"

#define MAX_SIZE 	2000000

enum header_types {
	NAME, TYPE, COMMENT, DIMENSION, DISPLAY, EDGE
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
	}
	else if (strcmp(title, "TYPE") == 0) {
		rtn = TYPE;
	}
	else if (strcmp(title, "COMMENT") == 0) {
		rtn = COMMENT;
	}
	else if (strcmp(title, "DIMENSION") == 0) {
		rtn = DIMENSION;
	}
	else if (strcmp(title, "DISPLAY_DATA_TYPE") == 0) {
		rtn = DISPLAY;
	}
	else if (strcmp(title, "EDGE_WEIGHT_TYPE") == 0) {
		rtn = EDGE;
	}
	else {
		rtn = -1;
	}
	return rtn;
}

void get_desipt(FILE * ofp, char *line) {
	char curr_c = 'f';
	int index = 0;

	while (curr_c != '\n' && curr_c != '\0') {
		curr_c = fgetc(ofp);
		if (index == 0 && curr_c == ' ') {
		}
		else if ((curr_c != '\n' && curr_c != '\0')) {
			line[index] = curr_c;
			index++;
		}
	}

}

int input_cities(FILE * ofp, struct tspfile * file) {
	int numAdded = 0;
	char line[100];

	fgets(line, sizeof(line), ofp);
	fgets(line, sizeof(line), ofp);
	printf("\tPopulation:\n");
	while (feof(ofp) == 0) {
		int index;
		float tempx, tempy;
		fgets(line, sizeof(line), ofp);

		sscanf(line, "%d %f %f", &index, &tempx, &tempy);
		if (index == numAdded + 1) {
			file->cities[numAdded].id = index;
			file->cities[numAdded].y = tempy;
			file->cities[numAdded].x = tempx;
			printf("\t\tCity ID: %02d at %f,%f\n", file->cities[numAdded].id,
					file->cities[numAdded].x, file->cities[numAdded].y);
			numAdded++;
		}

	}

	return numAdded;
}

int parse_file(char * file_name, struct tspfile * file) {
	FILE *ofp;
	char line[200];
	int type;

	if ((ofp = fopen(file_name, "r")) == NULL) {
		sprintf(line, "lib/%s.tsp", file_name);
		if ((ofp = fopen(line, "r")) == NULL) {
			printf("fail to open file\n");
			return -1;

		}
	}
	printf("TSP info\n");
	while ((type = get_lien_type(ofp)) != -1) {
		memset((void*) line, 0, sizeof(line));
		get_desipt(ofp, line);
		switch (type) {
		case NAME:
			strcpy(file->name, line);
			printf("\tName: %s\n", file->name);
			break;
		case TYPE:
			strcpy(file->type, line);
			printf("\tType: %s\n", file->type);
			break;
		case COMMENT:
			strcpy(file->comment, line);
			printf("\tComment: %s\n", file->comment);
			break;
		case DIMENSION:
			file->dimension = atoi(line);
			file->cities = malloc(file->dimension * sizeof(struct city));
			printf("\tSize: %d\n", file->dimension);
			break;
		case EDGE:
			strcpy(file->edge_weight_type, line);
			printf("\tEdge weighting: %s\n", file->edge_weight_type);
			break;
		case DISPLAY:
			strcpy(file->display_data_type, line);
			printf("\tDisplay tpye: %s\n", file->display_data_type);
			break;
		}
	}

	if (file->dimension < MAX_SIZE) {
		input_cities(ofp, file);
	}
	else {
		return -1;
	}
	printf("TSP file read success\n");
	return 0;
}

int get_opt(char * file){
	FILE *ofp;
	char line[100];
	char *title;
	char *dis_string;

	if(!(ofp = fopen("all.opt", "r"))){
		return 0;
	}
	while(fgets(line,100,ofp)) {
		title = strtok_r(line,":",&dis_string);
		if(!strcmp(file,title))
			break;
	}

	return atoi(dis_string);
}
