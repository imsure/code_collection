#include <stdio.h>
#include <stdlib.h>

struct tree_el {
	int val;
	struct tree_el *left;
	struct tree_el *right;
};

typedef struct tree_el node;

void 
insert (node **tree, node *item)
{
	if (!(*tree)) {
		*tree = item;
		return;
	}
	if (item->val < (*tree)->val) {
		insert (&(*tree)->left, item);
	}
	else if (item->val > (*tree)->val) {
		insert (&(*tree)->right, item);
	}
}

void
print_out (node *tree)
{
	if (tree->left) {
		print_out (tree->left);
	}
	printf ("%d\n", tree->val);
	if (tree->right) {
		print_out (tree->right);
	}
}

int 
main (int argc, char *argv[])
{
	node *curr, *root;
	int i;

	root = NULL;

	for (i=1; i<=10; i++) {
		curr = (node *)malloc (sizeof(node));
		curr->left = curr->right = NULL;
		//curr->val = rand();
		curr->val = i;
		insert (&root, curr);
	}

	print_out (root);

	return 0;
}
