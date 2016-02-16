#include "init.h"
#include "selforg.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void display_help();
struct node *setup_nodes(struct tspfile *map, u_int neighbours);

int main(int argc, char **argv) {
	char *file_name;
	u_int cycles = 0;
	u_int neighbours = 0;
	_Bool rand = false;
	struct tspfile file;
	u_int jpc = 0;
	char option;

	printf("Processing cmd args...\n");
	while ((option = getopt(argc, argv, "f:c:n:j:hr")) != -1) {
		switch (option) {

		case 'f':
			file_name =  optarg;
			break;
		case 'c':
			cycles = atoi(optarg);
			break;
		case 'n':
			neighbours = atoi(optarg);
			break;
		case 'j':
			jpc = atoi(optarg);
			break;
		case 'h':
			display_help();
			break;
		case 'r':
			rand = true;
			break;
		default:
			printf("Unrecognised option %c\n", option);
			display_help();
			break;
		}
	}
	printf("Done %d\n",neighbours);
	if ((parse_file(file_name, &file)) != -1) {
		struct node *nodes;
		if(jpc == 0){
			jpc = file.dimension/2;
		}
		if(neighbours == 0){
			neighbours = file.dimension/10;
		}
		if(cycles == 0){
			cycles = file.dimension;
		}
		nodes = setup_nodes(&file, neighbours);
		init_list(nodes,file.dimension);
		start_selforg(neighbours, jpc, cycles,rand);
	}
	return 0;
}

void display_help() {
	printf("Usage: selforg [OPTION...] \n");
	printf("\t-f <file_name>\tTSPLIB file (.tsp file) using cord format\n");
	printf(
			"\t-c <cycles>\tNumber of times algorium will be appied to each population member\n");
	printf("\t-n <neighs>\tNeightbourhood size\n");
	printf("\t-j <jump>\tMax distance a member can jump in a simple cycle\n");

}

double calc_dis(int x, int y, int x2, int y2) {
	double linea, lineb;
	double linec = 4.0;
	linea = abs(x - x2);
	lineb = abs(y - y2);
	linec = pow(linea, 2.0) + pow(lineb, 2.0);
	linec = sqrt(linec);
	return linec;
}

void populate_matrix(struct tspfile *map, double ** matrix) {
	register int i, y;
	int size = map->dimension;
	for (i = 0; i < size; i++) {
		for (y = 0; y < size; y++) {
			matrix[i][y] = calc_dis(map->cities[i].x, map->cities[i].y,
					map->cities[y].x, map->cities[y].y);
		}
	}
	//return true;
}

static void sort_assending(double *distance, struct node **nodes,u_int size){
	int i;
	for(i = size - 2; i >= 0 ; i--){
		if(distance[i] > distance[i + 1]){
			double tmpd;
			struct node *tmpn;
			tmpd = distance[i];
			tmpn = nodes[i];
			distance[i] = distance[i + 1];
			nodes[i] = nodes[i + 1];
			distance[i + 1] = tmpd;
			nodes[i + 1] = tmpn;
		}
	}
}

static void sort_desending(double *distance, struct node **nodes,u_int size){
	int i;
	for(i = 0; i < size- 1; i++){
		if(distance[i] < distance[i + 1]){
			double tmpd;
			struct node *tmpn;
			tmpd = distance[i];
			tmpn = nodes[i];
			distance[i] = distance[i + 1];
			nodes[i] = nodes[i + 1];
			distance[i + 1] = tmpd;
			nodes[i + 1] = tmpn;
		}
	}

}

void calculate_neighbours(struct node*nodes, u_int size, u_int neighbours) {
	register int i, y, z;
	double *shortest = (double*) malloc(sizeof(double) * neighbours);
	for (i = 0; i < size; i++) {

		for (z = 0; z < neighbours; z++) {
			shortest[z] = HUGE_VAL;

		}

		for (y = 0; y < size; y++) {
			if (i != y) {
				double dis = calc_dis(nodes[i].id->x, nodes[i].id->y, nodes[y].id->x,
						nodes[y].id->y);


				if (dis < shortest[neighbours -1]) {
					shortest[neighbours - 1] = dis;
					nodes[i].neighbours[neighbours - 1] = &nodes[y];
					sort_assending(shortest, nodes[i].neighbours,neighbours);
				}

			}
		}

		printf("node %d: ", i +1);
		for (z = 0; z < neighbours; z++) {
			printf("%d ", nodes[i].neighbours[z]->pos +1);
		}
		printf("\n");

	}
	free(shortest);
}

struct node *setup_nodes(struct tspfile *map, u_int neighbours) {
	register int i = 0;
	struct node *nodes;
	nodes = (struct node*)malloc(sizeof(struct node)*map->dimension);
	for(i = 0; i < map->dimension; i++){
		nodes[i].id = &map->cities[i];
		nodes[i].pos = i;
		nodes[i].neighbours = (struct node**)malloc(sizeof(struct node*)*map->dimension);
	}

	calculate_neighbours(nodes, map->dimension, neighbours);
	return nodes;
}

