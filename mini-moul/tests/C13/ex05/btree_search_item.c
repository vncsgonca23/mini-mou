#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

void	*btree_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *));

static int	cmp_first_char(void *a, void *b)
{
	return ((unsigned char)((char *)a)[0] - (unsigned char)((char *)b)[0]);
}

static t_btree	*mk(void *item, t_btree *left, t_btree *right)
{
	t_btree	*n = malloc(sizeof(t_btree));
	n->item = item;
	n->left = left;
	n->right = right;
	return (n);
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
	int	error = 0;

	if (btree_search_item(NULL, "A", &cmp_first_char) != NULL)
	{
		printf("    " RED "[1] Empty tree expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty tree -> NULL\n" DEFAULT);

	/* Tree:       "X_root"
	 *            /        \
	 *       "X_left"     "Y_right"
	 * Infix visits X_left BEFORE X_root, so searching "X" MUST return "X_left"!
	 */
	char	*x_left = "X_left";
	char	*x_root = "X_root";
	char	*y_right = "Y_right";
	t_btree	*root = mk(x_root, mk(x_left, NULL, NULL), mk(y_right, NULL, NULL));

	void	*found = btree_search_item(root, "X", &cmp_first_char);
	if (found != x_left)
	{
		printf("    " RED "[2] Infix priority failed: expected \"%s\", got \"%s\"\n" DEFAULT,
			x_left, found ? (char *)found : "NULL");
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Returned first match in infix order (\"X_left\")\n" DEFAULT);

	if (btree_search_item(root, "Y", &cmp_first_char) != y_right)
	{
		printf("    " RED "[3] Right subtree search failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Found item in right subtree\n" DEFAULT);

	if (btree_search_item(root, "Z", &cmp_first_char) != NULL)
	{
		printf("    " RED "[4] Absent item expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] Absent item -> NULL\n" DEFAULT);

	free_tree(root);
	return (error);
}
