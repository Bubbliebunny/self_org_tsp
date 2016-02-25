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
static void init_list(struct list*all_list, struct tspfile* map,
		u_int neighbours, u_int repel);
static void calculate_neighbours(struct list *all_list, u_int neighbours,
		u_int repel);
static void load_N_from_file(FILE *ofp, struct list*all_list, u_int neighbours,
		u_int repel);
static void wrtie_N_to_file(FILE *ofp, struct list* all_list, u_int neighbours);
static int parse_args(int argc, char** argv, u_int *neighbours, u_int *repel, u_int *cycle);

int main(int argc, char **argv) {
	char *file_name;
	u_int neighbours = 0;
	u_int repel = 0;
	u_int cycle = 0;
	struct tspfile file;
	struct list all_list;
	FILE *ofp;
	char n_file[100];

	if((file_name = parse_args(argc, argv, &neighbours, &repel, &cycle)) == NULL){
		exit(1);
	}

	if ((parse_file(file_name, &file)) != -1) {
		if(neighbours >= file.dimension){
			exit (1);
		}
		init_list(&all_list, &file, file.dimension-neighbours, repel);
		sprintf(n_file, "Neighbours/N-%s.tsp", file.name);
		if ((ofp = fopen(n_file, "r")) != NULL) {
			load_N_from_file(ofp, &all_list, file.dimension-neighbours, repel);
			fclose(ofp);
		}
		else {
			printf("Unable to find pre-process file, continue?");
			getchar();
			calculate_neighbours(&all_list, neighbours, repel);
			getchar();
		}
		sprintf(n_file, "results/R-%s.tsp", file.name);
		if ((ofp = fopen(n_file, "a")) != NULL) {
			int opt = get_opt(file.name);
			double result = start_selforg(&all_list, file.dimension-neighbours, repel,cycle );

			fprintf(ofp,"N:%d C:%d best:%f%%\n",file.dimension-neighbours,cycle,(result / (double)opt) * 100);
			fclose(ofp);
		}

	}
	return 0;
}

static int parse_args(int argc, char** argv, u_int *neighbours, u_int *repel,u_int *cycle){
	char option;
	_Bool n_found = false;
	_Bool r_found = false;
	_Bool c_found = false;
	char* file_name;

	printf("Processing cmd args...\n");
	while ((option = getopt(argc, argv, "f:n:hr:c:")) != -1) {
		switch (option) {
		case 'f':
			file_name = optarg;
			break;
		case 'n':
			n_found = true;
			*neighbours = atoi(optarg);
			break;
		case 'c':
			c_found = true;
			*cycle = atoi(optarg);
			break;
		case 'h':
			display_help();
			break;
		case 'r':
			r_found = true;
			*repel = atoi(optarg);
			break;
		default:
			printf("Unrecognised option %c\n", option);
			display_help();
			return 0;
			break;
		}
	}
	if (!n_found) {
		return 0;
	}
	if (!r_found) {
		return 0;
	}
	if (!c_found) {
		return 0;
	}
	return file_name;
}


static void display_help() {
	printf("Usage: selforg [OPTION...] \n");
	printf("\t-f <file_name>\tTSPLIB file (.tsp file) using cord format\n");
	printf(
			"\t-c <cycles>\tNumber of times algorium will be appied to each population member\n");
	printf("\t-n <neighs>\tNeightbourhood size\n");
	printf("\t-j <jump>\tMax distance a member can jump in a simple cycle\n");

}

double inline calc_dis(int x, int y, int x2, int y2) {
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

static void inline sort_assending(double *distance, struct node **nodes,
		u_int size) {
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
	for (i = size - 1; i > 0; i--) {
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

static void calculate_neighbours(struct list *all_list, u_int neighbours,
		u_int repel) {
	struct list_elem *curr_elem = all_list->head;
	double *shortest = (double*) malloc(sizeof(double) * neighbours);
	double *fairest = (double*) malloc(sizeof(double) * repel);
	double dis;
	struct list_elem *comp_elem;
	register int i;

	do {

		comp_elem = curr_elem->right;

		for (i = 0; i < neighbours; i++) {
			shortest[i] = HUGE_VAL;

		}
		for (i = 0; i < repel; i++) {

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

			if (dis > fairest[repel - 1]) {
				fairest[repel - 1] = dis;
				curr_elem->data->remote[repel - 1] = comp_elem->data;
				sort_desending(fairest, curr_elem->data->remote, repel);
			}

			comp_elem = comp_elem->right;
		} while (comp_elem != curr_elem);
		curr_elem = curr_elem->right;

	} while (curr_elem != all_list->head);

	free(shortest);
	free(fairest);
}

static struct node *setup_node(struct city *id, u_int neighbours, u_int repel) {
	struct node *new_node;
	new_node = (struct node*) malloc(sizeof(struct node));
	new_node->id = id;
	new_node->pos = 0;
	new_node->neighbours = (struct node**) malloc(
			sizeof(struct node*) * neighbours);
	new_node->remote = (struct node**) malloc(sizeof(struct node*) * repel);
	return new_node;
}

static void init_list(struct list*all_list, struct tspfile* map,
		u_int neighbours, u_int repel) {
	struct list_elem *new_elem;
	struct list_elem *last_elem;
	int index = 0;

	assert(map->dimension != 0);

	all_list->length = map->dimension;

	new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
	new_elem->data = (struct node*) setup_node(&map->cities[index], neighbours,
			repel);
	last_elem = new_elem;
	index++;
	all_list->head = new_elem;
	while (index < map->dimension) {
		new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
		new_elem->data = (struct node*) setup_node(&map->cities[index], neighbours,
				repel);
		new_elem->left = last_elem;
		last_elem->right = new_elem;
		last_elem = new_elem;
		index++;
	}
	all_list->head->left = last_elem;
	last_elem->right = all_list->head;
}

static void wrtie_N_to_file(FILE *ofp, struct list *all_list, u_int neighbours) {
	struct list_elem * curr = all_list->head;

	do {
		fprintf(ofp, "%d:", curr->data->id->id);
		for (int i = 0; i < neighbours; i++) {
			fprintf(ofp, "%d,", curr->data->neighbours[i]->id->id);
		}
		fprintf(ofp, "\n");
		curr = curr->right;
	} while (curr != all_list->head);
}

static struct node *get_node_by_ID(struct list*all_list, int id) {
	struct list_elem * curr = all_list->head;
	do {
		if (id == curr->data->id->id) {
			return curr->data;
		}
		curr = curr->right;
	} while (curr != all_list->head);
	return NULL;
}

static void load_N_from_file(FILE *ofp, struct list*all_list, u_int neighbours,
		u_int repel) {
	struct list_elem * curr = all_list->head;
	int catch;

	do {
		fscanf(ofp, "%d:", &
		catch);
		for (int i = 0; i < all_list->length - 1; i++) {
			fscanf(ofp, "%d,", &
			catch);
			if (i < neighbours) {
				curr->data->neighbours[i] = get_node_by_ID(all_list, catch);
			}
			if (i >= (all_list->length - 1)  - repel) {
				curr->data->remote[(all_list->length - (i + 1)) - 1] = get_node_by_ID(all_list, catch);
			}
		}
		curr = curr->right;
	} while (curr != all_list->head);
}
