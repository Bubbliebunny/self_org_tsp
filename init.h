#include "tsplib.h"

#ifndef __INIT_H__
#define __INIT_H__

typedef unsigned int u_int;

typedef struct node node;

struct node {
	struct city *id;
	int pos;
	struct node** neighbours;
};



#endif //__INIT_H_
