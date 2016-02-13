#include <assert.h>
#include "selforg.h"
#include <stdbool.h>

typedef struct list_elem list_elem;

struct list_elem{
	struct node *data;
	struct list_elem* left;
	struct list_elem* right;
};

struct list {
	struct list_elem* head;
	int length;
};

struct list all_list;

static void printf_sequence();

void init_list(struct node* nodes, u_int size){
	struct list_elem *new_elem;
	struct list_elem *last_elem;
	int index = 0;

	assert(nodes != NULL);
	assert(size != 0);

	all_list.length = size;

	new_elem = (struct list_elem *) malloc (sizeof(struct list_elem));
	new_elem->data = &nodes[index];
	last_elem = new_elem;
	index++;
	all_list.head = new_elem;
	while(index < size){
		new_elem = (struct list_elem *) malloc (sizeof(struct list_elem));
		new_elem->data = &nodes[index];
		new_elem->left = last_elem;
		last_elem->right = new_elem;
		last_elem = new_elem;
		index++;
	}

	all_list.head->left = last_elem;
	last_elem->right = all_list.head;
}

void start_selforg(u_int neighbours, u_int jump, u_int cycles,_Bool rand){

}

static void printf_sequence(){
	struct list_elem * curr;
	curr= all_list.head;
	for(int i = 0; i < 100; i++){
		printf("%d-",curr->data->id->id);
		curr = curr->right;
	}
	printf("\n");
}

