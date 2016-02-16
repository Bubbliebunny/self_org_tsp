#include "init.h"

#ifndef __SELFORG_H__
#define __SELFORG_H__

void init_list(struct node* nodes, u_int size);

void start_selforg(u_int neighbours, u_int jump, u_int cycles,_Bool randstart);



#endif //__SELFORG_H__
