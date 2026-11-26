#include <stdio.h>
#include <stdlib.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

int	btree_level_count(t_btree *root);

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

	if (btree_level_count(NULL) != 0)
	{
		printf("    " RED "[1] Empty tree Expected 0, got %d\n" DEFAULT, btree_level_count(NULL));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty tree -> 0\n" DEFAULT);

	t_btree	*one = mk("A", NULL, NULL);
	if (btree_level_count(one) != 1)
	{
		printf("    " RED "[2] Single node Expected 1, got %d\n" DEFAULT, btree_level_count(one));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single node -> 1\n" DEFAULT);
	free_tree(one);

	/* Skewed tree: left depth 2, right depth 4 -> max 4 */
	t_btree	*skew = mk("1",
		mk("2L", NULL, NULL),
		mk("2R", NULL, mk("3R", mk("4RL", NULL, NULL), NULL)));
	if (btree_level_count(skew) != 4)
	{
		printf("    " RED "[3] Unbalanced tree Expected 4, got %d\n" DEFAULT, btree_level_count(skew));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Unbalanced tree (depth 4) -> 4\n" DEFAULT);
	free_tree(skew);

	/* Left-deep tree: depth 3 */
	t_btree	*left_deep = mk("1", mk("2", mk("3", NULL, NULL), NULL), NULL);
	if (btree_level_count(left_deep) != 3)
	{
		printf("    " RED "[4] Left-skewed tree Expected 3, got %d\n" DEFAULT, btree_level_count(left_deep));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] Left-skewed tree (depth 3) -> 3\n" DEFAULT);
	free_tree(left_deep);

	return (error);
}
