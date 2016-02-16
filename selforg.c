#include <assert.h>
#include "selforg.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct list_elem list_elem;

struct list_elem {
	struct node *data;
	struct list_elem* left;
	struct list_elem* right;
};

struct list {
	struct list_elem* head;
	int length;
};

struct list all_list;
int ldis = 0;
int rdis = 0;
double losg = HUGE_VAL;

static void printf_sequence();

void init_list(struct node* nodes, u_int size) {
	struct list_elem *new_elem;
	struct list_elem *last_elem;
	int index = 0;

	assert(nodes != NULL);
	assert(size != 0);

	all_list.length = size;

	new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
	new_elem->data = &nodes[index];
	last_elem = new_elem;
	index++;
	all_list.head = new_elem;
	while (index < size) {
		new_elem = (struct list_elem *) malloc(sizeof(struct list_elem));
		new_elem->data = &nodes[index];
		new_elem->left = last_elem;
		last_elem->right = new_elem;
		last_elem = new_elem;
		index++;
	}

	all_list.head->left = last_elem;
	last_elem->right = all_list.head;
}

static _Bool check_neighbour(node *c_node, node *check, int neighbours) {
	for (int i = 0; i < neighbours; i++) {
		if (c_node->neighbours[i] == check) {
			return true;
		}
	}
	return false;
}

static void move(list_elem *node, int places) {
	struct list_elem * curr = node;
	node->left->right = node->right;
	node->right->left = node->left;

	while (places != 0) {
		if (places < 0) {
			curr = curr->left;
			places++;
		}
		else {
			curr = curr->right;
			places--;
		}
	}
	node->left = curr;
	node->right = curr->right;
	node->left->right = node;
	node->right->left = node;

}

// 1 = right, 0 = left
static int determine_direction(list_elem * curr_node, int neighbours) {
	int left = 0;
	int right = 0;
	int count = 0;

	struct list_elem *left_node = curr_node->left;
	struct list_elem *right_node = curr_node->right;

	while (1) {
		if (check_neighbour(curr_node->data, left_node->data, neighbours)) {
			left++;
			ldis += (count +1);
		}
		else if (check_neighbour(curr_node->data, right_node->data, neighbours)) {
			right++;
			rdis += (count +1);
		}
		if ((left + right) >= neighbours)
			break;
		left_node = left_node->left;
		right_node = right_node->right;
		count++;
	}

	return (right > left);
}

void start_selforg(u_int neighbours, u_int jump, u_int cycles, _Bool randstart) {
	struct list_elem * curr;
	int dir;
	srand(23);
	for (int y = 0; y < cycles; y++) {
		curr= all_list.head;
		for (int i = 0; i < all_list.length; i++) {
			dir = determine_direction(curr, neighbours);
			int jmp = 0;
			if (dir == 1) {
				jmp = ((rdis / neighbours)/2);
				if(jmp > jump)
					jmp = jump;
				move(curr, jmp);
			}
			else {
				jmp = ((ldis / neighbours)/2);
				if(jmp > jump)
					jmp = jump;
				jmp++;
				move(curr, -jmp);
			}
			rdis = ldis = 0;
			curr = curr->right;
			printf_sequence();
		}
		/*
		if(((y+1) % 1000) == 0){
			neighbours-=2;
			printf("new neighbours: %d\n",neighbours);
		}*/
	}
	FILE *ofp;
	ofp = fopen("eil51_10kcycle_fixedN_Javgdis.txt","a");
	fprintf(ofp,"N: %d, Jcap: %d, Best: %f\n",neighbours,jump,losg);
	printf("N: %d, Jcap: %d, Best: %f\n",neighbours,jump,losg);
	fclose(ofp);
}

static void printf_sequence() {
	struct list_elem * curr;
	double dis = 0;
	curr = all_list.head;
	for (int i = 0; i < all_list.length; i++) {
		//printf("%d-", curr->data->id->id);
		dis += calc_dis(curr->data->id->x, curr->data->id->y,
				curr->right->data->id->x, curr->right->data->id->y);
		curr = curr->right;
	}
	if(dis < losg)
		losg = dis;

	//printf("%f\n", dis);
}

