
void
insert_node (struct node **root, struct node* nd)
{
	if (*root == NULL) {
		*root = nd;
	} else {
		if (nd->data < (*root)->data) {
			insert_node (&((*root)->left), nd);
		} else {
			insert_node (&((*root)->right), nd);
		}
	}
}
