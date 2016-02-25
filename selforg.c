#include "selforg.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "draw.h"

int left_dis = 0;
int right_dis = 0;
double losg = HUGE_VAL;
static void printf_sequence(struct list*);
static _Bool move_away(struct list *all_list);
static double track_best(struct list* all_list);


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
	if (places != 0) {
		toMove->left->right = toMove->right;
		toMove->right->left = toMove->left;

		//list elements are slotted into the right, this equilizes the jumps for left and right
		if (places < 0)
			places--;
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
}

// 1 = right, 0 = left
static int determine_direction(list_elem * curr_node, u_int neighbours) {
	int left = 0;
	int right = 0;
	int count = 1;

	struct list_elem *left_node = curr_node->left;
	struct list_elem *right_node = curr_node->right;

	while ((left << 1) < neighbours && (right << 1) < neighbours) {
		if (check_neighbour(curr_node->data, left_node->data, neighbours)) {
			if (left == 0)
				left_dis = (count / 2) + 1;
			left++;
		}
		if (check_neighbour(curr_node->data, right_node->data, neighbours)) {
			if (right == 0)
				right_dis = (count / 2) + 1;
			right++;
		}
		left_node = left_node->left;
		right_node = right_node->right;
		count++;
	}

	return (right > left);
}

double start_selforg(struct list *all_list, u_int neighbours, u_int repel,u_int cycle) {
	struct list_elem * curr;
	u_int start_neighbours = neighbours;
	struct list_elem **nodes = malloc(
			sizeof(struct list_elem*) * all_list->length);

	curr = all_list->head;

	for (int i = 0; i < all_list->length; i++) {
		nodes[i] = curr;
		curr = curr->right;
	}
	for (int y = 1; y < all_list->length  * (all_list->length * cycle); y++) {
		if (determine_direction(nodes[y % all_list->length], neighbours)) {
			move(nodes[y % all_list->length], right_dis);
		}
		else {
			move(nodes[y % all_list->length], -left_dis);
		}
		right_dis = 0;
		left_dis = 0;
		track_best(all_list);
		//draw_map(all_list);
		//move_away(all_list);

		if (neighbours > 3) {
			if ((y % (all_list->length * cycle)) == 0) {
				neighbours -= 1;
			}
		}
	}
	printf_sequence(all_list);
	return 	track_best(all_list);
}

static double track_best(struct list* all_list) {
	static double best = HUGE_VAL;
	double dis;
	struct list_elem * curr;
		curr = all_list->head;
	for (int i = 0; i < all_list->length; i++) {
		dis += calc_dis(curr->data->id->x, curr->data->id->y,
				curr->right->data->id->x, curr->right->data->id->y);
		curr = curr->right;
	}

	if (dis < best)
		best = dis;

	return best;
}
static void printf_sequence(struct list* all_list) {
	struct list_elem * curr;
	curr = all_list->head;
	for (int i = 0; i < all_list->length; i++) {
		printf("%d-", curr->data->id->id);
		curr = curr->right;
	}
	printf("\n");
}

static _Bool move_away(struct list *all_list) {
	struct list_elem *curr_elem = all_list->head;
	do {
		if (curr_elem->data->remote[0] == curr_elem->right->data) {
			printf("%d - %d\n", curr_elem->data->id->id,
					curr_elem->right->data->id->id);
			move(curr_elem, -(all_list->length / 2));

		}
		if (curr_elem->data->remote[0] == curr_elem->left->data) {
			printf("%d - %d\n", curr_elem->data->id->id,
					curr_elem->left->data->id->id);
			move(curr_elem, all_list->length / 2);

		}
		if (curr_elem->data->remote[1] == curr_elem->right->data) {
			printf("%d - %d\n", curr_elem->data->id->id,
					curr_elem->right->data->id->id);
			move(curr_elem, -(all_list->length / 2));

		}
		if (curr_elem->data->remote[1] == curr_elem->left->data) {
			printf("%d - %d\n", curr_elem->data->id->id,
					curr_elem->left->data->id->id);
			move(curr_elem, all_list->length / 2);

		}

		curr_elem = curr_elem->right;
	} while (curr_elem != all_list->head);

}

