
#ifndef ___TSPLIB_H__
#define __TSPLIB_H__

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct node node;

struct node{
	int pos;
	int id;
	float x;
	float y;
	node *neigh[100];
	node *left;
	node *right;
};

typedef struct {
	char name[20];
	char type[20];
	char comment[200];
	int dimension;
	char display_data_type[20];
	char edge_weight_type[20];
	int neigh_count;
	node *nodes;
} tspfile;
int parse_file(char * file_name, tspfile * file); 

#endif