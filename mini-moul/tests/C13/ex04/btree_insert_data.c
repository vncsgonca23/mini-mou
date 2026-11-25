#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

void	btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *));

t_btree	*btree_create_node(void *item)
{
	t_btree	*n = malloc(sizeof(t_btree));
	if (!n)
		return (NULL);
	n->item = item;
	n->left = NULL;
	n->right = NULL;
	return (n);
}

static int	cmp_str(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b));
}

static void	collect_infix(t_btree *r, char *out)
{
	if (!r)
		return ;
	collect_infix(r->left, out);
	strcat(out, (char *)r->item);
	collect_infix(r->right, out);
}

static void	free_tree(t_btree *r)
{
	if (!r)
		return ;
	free_tree(r->left);
	free_tree(r->right);
	free(r);
}

int	main(void)
{
	int		error = 0;
	t_btree	*root = NULL;

	/* Test 1: Insert into empty tree */
	btree_insert_data(&root, "M", &cmp_str);
	if (!root || strcmp((char *)root->item, "M") != 0 || root->left || root->right)
	{
		printf("    " RED "[1] Insert into empty tree failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Insert into empty tree created root\n" DEFAULT);

	/* Test 2: Insert smaller (< -> left), larger (> -> right), and EQUAL (== -> right!) */
	btree_insert_data(&root, "D", &cmp_str);
	btree_insert_data(&root, "T", &cmp_str);
	btree_insert_data(&root, "A", &cmp_str);
	btree_insert_data(&root, "G", &cmp_str);
	btree_insert_data(&root, "M", &cmp_str);

	char	buf[64] = {0};
	collect_infix(root, buf);
	if (strcmp(buf, "ADGMMT") != 0)
	{
		printf("    " RED "[2] Infix order Expected \"ADGMMT\", got \"%s\"\n" DEFAULT, buf);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Infix order matched \"ADGMMT\"\n" DEFAULT);

	/* Verify equal element "M" went to the RIGHT subtree of root (per subject: >= goes right) */
	if (!root->right || !root->right->left || strcmp((char *)root->right->left->item, "M") != 0)
	{
		printf("    " RED "[3] Equal element was not inserted into right subtree\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Equal element placed in right subtree (>= goes right)\n" DEFAULT);

	free_tree(root);
	return (error);
}
