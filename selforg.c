
#include "selforg.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"


int left_dis = 0;
int right_dis = 0;
double losg = HUGE_VAL;
static void printf_sequence(struct list*);
static _Bool repel(struct list *all_list);
static _Bool check_neighbour(node *c_node, node *check, int neighbours) {
	for (int i = 0; i < neighbours; i++) {
		if (c_node->neighbours[i] == check) {
			return true;
		}
	}
	return false;
}

static void move(struct list_elem *toMove, int places) {
	struct list_elem * curr = toMove;
	int step_count = 0;
	toMove->left->right = toMove->right;
	toMove->right->left = toMove->left;

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
	toMove->left = curr;
	toMove->right = curr->right;
	toMove->left->right = toMove;
	toMove->right->left = toMove;

}

// 1 = right, 0 = left
static int determine_direction(list_elem * curr_node, u_int neighbours) {
	int left = 0;
	int right = 0;
	int count = 1;

	struct list_elem *left_node = curr_node->left;
	struct list_elem *right_node = curr_node->right;

	while (left + right < neighbours) {
		if (check_neighbour(curr_node->data, left_node->data, neighbours)) {
			if(left == 0)
				left_dis = (count / 2)  + 3;
			left++;
		}
		if (check_neighbour(curr_node->data, right_node->data, neighbours)) {
			if(right == 0)
				right_dis = (count / 2) + 2;
			right++;
		}
		left_node = left_node->left;
		right_node = right_node->right;
		count++;
	}

	return (right > left);
}

void start_selforg(struct list *all_list, u_int neighbours, u_int jump,
		u_int cycles, _Bool randstart) {
	struct list_elem * curr;
	int dir;
	struct list_elem **nodes = malloc(sizeof(struct list_elem*)*all_list->length);
	_Bool bounce = true;
	curr = all_list->head;
	time_t t;
   srand((unsigned) time(&t));

	for(int i = 0; i < all_list->length; i++){
		nodes[i] = curr;
		curr = curr->right;
	}
	for (int y = 1; y < cycles; y++) {
		dir = determine_direction(nodes[y % all_list->length], neighbours);
		if (dir == 1) {
			move(nodes[y % all_list->length], right_dis);
		}
		else {
			move(nodes[y % all_list->length], -left_dis);
		}
		right_dis = 0;
		left_dis = 0;
		//curr = next_st->left;
		printf_sequence(all_list);
		draw_map(all_list);
		repel(all_list);

		/*
		if(bounce) {
			if((y % 100) == 0)
				neighbours -= 2;
			if(neighbours < 3){
				neighbours = 3;
				bounce = false;
			}
		} else {
			if((y % 100) == 0)
				neighbours += 2;
			if(neighbours > all_list->length - 2){
				neighbours = all_list->length - 2;
				bounce = true;
			}
			//break;
		}	*/
	}

//FILE *ofp;
//ofp = fopen("eil51_10kcycle_fixedN_Javgdis.txt", "a");
//fprintf(ofp,"N: %d, Jcap: %d, Best: %f\n",neighbours,jump,losg);
	printf("\nN: %d, Jcap: %d, Best: %f\n",neighbours,jump,losg);
//	fclose( ofp);

}

static void printf_sequence(struct list* all_list) {
struct list_elem * curr;
double dis = 0;
curr = all_list->head;
for (int i = 0; i < all_list->length; i++) {
	printf("%d-", curr->data->id->id);
	dis += calc_dis(curr->data->id->x, curr->data->id->y,
			curr->right->data->id->x, curr->right->data->id->y);
	curr = curr->right;
}
if (dis < losg)
	losg = dis;

printf("%f\n", dis);
}

static _Bool repel(struct list *all_list){
	struct list_elem *curr_elem = all_list->head;
	do {
		if(curr_elem->data->remote[0] == curr_elem->right->data ){
			move(curr_elem, -(all_list->length/2));
		}
		if(curr_elem->data->remote[0] == curr_elem->left->data ){
			move(curr_elem, all_list->length/2);
		}

		curr_elem = curr_elem->right;
	} while(curr_elem != all_list->head);

}

//static void subset(struct list all_list, u_int neighbours){


