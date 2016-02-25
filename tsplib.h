
#ifndef __TSPLIB_H__
#define __TSPLIB_H__

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


struct city{
	int id;
	float x;
	float y;
};

struct tspfile{
	char name[50];
	char type[50];
	char comment[200];
	int dimension;
	char display_data_type[50];
	char edge_weight_type[50];
	int neigh_count;
	struct city *cities;
};

int parse_file(char * file_name, struct tspfile * file);
int get_opt(char * file);

#endif
