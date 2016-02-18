#include "tsplib.h"

#ifndef __INIT_H__
#define __INIT_H__

typedef unsigned int u_int;

typedef struct node node;
typedef struct list_elem list_elem;

struct node {
	struct city *id;
	int pos;
	struct node** neighbours;
	struct node** remote;
};

struct list_elem {
	struct node *data;
	struct list_elem* left;
	struct list_elem* right;
};

struct list {
	struct list_elem* head;
	int length;
};

double calc_dis(int x, int y, int x2, int y2);

#endif //__INIT_H_
