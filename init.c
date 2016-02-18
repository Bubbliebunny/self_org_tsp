#include "init.h"
#include "selforg.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

static void display_help();
static void init_list(struct list*all_list, struct tspfile* map, u_int neighbours);
static void calculate_neighbours(struct list *all_list, u_int neighbours);
static void subset(struct list all_list, u_int neighbours);
int main(int argc, char **argv) {
	char *file_name;
	u_int cycles = 0;
	u_int neighbours = 0;
	_Bool rand = false;
	struct tspfile file;
	u_int jpc = 0;
	char option;
	struct list all_list;

	printf("Processing cmd args...\n");
	while ((option = getopt(argc, argv, "f:c:n:j:hr")) != -1) {
		switch (option) {

		case 'f':
			file_name = optarg;
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
	printf("Done %d\n", neighbours);
	if ((parse_file(file_name, &file)) != -1) {
		if (jpc == 0) {
			jpc = file.dimension / 2;
		}
		if (neighbours == 0) {
			neighbours = file.dimension / 10;
		}
		if (cycles == 0) {
			cycles = file.dimension;
		}
		//nodes = setup_nodes(&file, neighbours);
		init_list(&all_list, &file, neighbours);
		calculate_neighbours(&all_list, neighbours);
		start_selforg(&all_list, neighbours, jpc, cycles, rand);

	}
	return 0;
}

static void display_help() {
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

static void populate_matrix(struct tspfile *map, double ** matrix) {
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

static void sort_assending(double *distance, struct node **nodes, u_int size) {
	int i;
	for (i = size - 1; i > 0; i--) {
		if (distance[i] < distance[i - 1]) {
			//printf("swap %f - %f\n",distance[i], distance[i-1]);
			double tmpd;
			struct node *tmpn;
			tmpd = distance[i];
			tmpn = nodes[i];
			distance[i] = distance[i - 1];
			nodes[i] = nodes[i - 1];
			distance[i - 1] = tmpd;
			nodes[i - 1] = tmpn;
		}
	}
}

static void sort_desending(double *distance, struct node **nodes, u_int size) {
	int i;
	for (i = size -1; i > 0; i--) {
		if (distance[i] > distance[i - 1]) {
			double tmpd;
			struct node *tmpn;
			tmpd = distance[i];
			tmpn = nodes[i];
			distance[i] = distance[i - 1];
			nodes[i] = nodes[i - 1];
			distance[i - 1] = tmpd;
			nodes[i - 1] = tmpn;
		}
	}

}

static void calculate_neighbours(struct list *all_list, u_int neighbours) {
	struct list_elem *curr_elem = all_list->head;
	double *shortest = (double*) malloc(sizeof(double) * neighbours);
	double *fairest = (double*) malloc(sizeof(double) * neighbours);
	double dis;
	struct list_elem *comp_elem;
	register int i;

	do {

		comp_elem = curr_elem->right;

		for (i = 0; i < neighbours; i++) {
			shortest[i] = HUGE_VAL;
			fairest[i] = 0.0;
		}

		do {
			dis = calc_dis(curr_elem->data->id->x, curr_elem->data->id->y,
					comp_elem->data->id->x, comp_elem->data->id->y);

			if (dis < shortest[neighbours - 1]) {
				shortest[neighbours - 1] = dis;
				curr_elem->data->neighbours[neighbours - 1] = comp_elem->data;
				sort_assending(shortest, curr_elem->data->neighbours, neighbours);

			}

			if(dis > fairest[neighbours - 1]){
				fairest[neighbours - 1] = dis;
				curr_elem->data->remote[neighbours - 1] = comp_elem->data;
				sort_desending(fairest, curr_elem->data->remote, neighbours);
			}

			comp_elem = comp_elem->right;
		} while (comp_elem != curr_elem);
		printf("%d: ",curr_elem->data->id->id);
		for (i = 0; i < neighbours; i++) {
			printf("%d ", curr_elem->data->neighbours[i]->id->id);
		}
		printf("\n");
		for (i = 0; i < neighbours; i++) {
			printf("%d ", curr_elem->data->remote[i]->id->id);
		}
		printf("\n");
		curr_elem = curr_elem->right;

	} while (curr_elem != all_list->head);

	free(shortest);
}

static struct node *setup_node(struct city *id, u_int neighbours) {
	struct node *new_node;
	new_node = (struct node*) malloc(sizeof(struct node));
	new_node->id = id;
	new_node->pos = 0;
	new_node->neighbours = (struct node**) malloc(
			sizeof(struct node*) * neighbours);
	new_node->remote = (struct node**) malloc(
				sizeof(struct node*) * neighbours);
	return new_node;
}

static void init_list(struct list*all_list, struct tspfile* map, u_int neighbours) {
	struct list_elem *new_elem;
	struct list_elem *last_elem;
	int index = 0;

	assert(map->dimension != 0);

	all_list->length = map->dimension;

	new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
	new_elem->data = (struct node*)setup_node(&map->cities[index], neighbours);
	last_elem = new_elem;
	index++;
	all_list->head = new_elem;
	while (index < map->dimension) {
		new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
		new_elem->data = (struct node*)setup_node(&map->cities[index], neighbours);
		new_elem->left = last_elem;
		last_elem->right = new_elem;
		last_elem = new_elem;
		index++;
	}
	all_list->head->left = last_elem;
	last_elem->right = all_list->head;
}

static void subset(struct list all_list, u_int neighbours){



}
