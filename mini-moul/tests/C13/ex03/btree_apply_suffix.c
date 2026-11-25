#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

void	btree_apply_suffix(t_btree *root, void (*applyf)(void *));

static char	g_buf[64];

static void	record(void *item)
{
	strcat(g_buf, (char *)item);
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

	g_buf[0] = '\0';
	btree_apply_suffix(NULL, &record);
	if (g_buf[0] != '\0')
	{
		printf("    " RED "[1] Empty tree invoked applyf\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty tree handled cleanly\n" DEFAULT);

	/* Tree:      A
	 *          /   \
	 *         B     C
	 *        / \     \
	 *       D   E     F
	 * Suffix (left, right, root): D E B F C A
	 */
	t_btree	*root = mk("A", mk("B", mk("D", NULL, NULL), mk("E", NULL, NULL)), mk("C", NULL, mk("F", NULL, NULL)));
	g_buf[0] = '\0';
	btree_apply_suffix(root, &record);
	if (strcmp(g_buf, "DEBFCA") != 0)
	{
		printf("    " RED "[2] Suffix traversal Expected \"DEBFCA\", got \"%s\"\n" DEFAULT, g_buf);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Suffix traversal matched \"DEBFCA\"\n" DEFAULT);
	free_tree(root);

	return (error);
}
