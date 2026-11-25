#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

void	btree_apply_by_level(t_btree *root, void (*applyf)(void *item, int current_level, int is_first_elem));

static char	g_log[256];

static void	record_level(void *item, int current_level, int is_first_elem)
{
	char	entry[32];

	snprintf(entry, sizeof(entry), "%s:%d:%d ", (char *)item, current_level, is_first_elem);
	strcat(g_log, entry);
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

	/* Test 1: Empty tree */
	g_log[0] = '\0';
	btree_apply_by_level(NULL, &record_level);
	if (g_log[0] != '\0')
	{
		printf("    " RED "[1] Empty tree invoked applyf\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty tree handled cleanly\n" DEFAULT);

	/* Test 2: Single node */
	t_btree	*one = mk("R", NULL, NULL);
	g_log[0] = '\0';
	btree_apply_by_level(one, &record_level);
	if (strcmp(g_log, "R:0:1 ") != 0)
	{
		printf("    " RED "[2] Single node Expected \"R:0:1 \", got \"%s\"\n" DEFAULT, g_log);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single node passed (\"R:0:1 \")\n" DEFAULT);
	free_tree(one);

	/* Test 3: Unbalanced multi-level tree
	 *        A            level 0: A(first=1)
	 *       / \
	 *      B   C          level 1: B(first=1), C(first=0)
	 *       \   \
	 *        D   E        level 2: D(first=1), E(first=0)
	 *           /
	 *          F          level 3: F(first=1)
	 */
	t_btree	*root = mk("A",
		mk("B", NULL, mk("D", NULL, NULL)),
		mk("C", NULL, mk("E", mk("F", NULL, NULL), NULL)));
	g_log[0] = '\0';
	btree_apply_by_level(root, &record_level);
	const char	*exp = "A:0:1 B:1:1 C:1:0 D:2:1 E:2:0 F:3:1 ";
	if (strcmp(g_log, exp) != 0)
	{
		printf("    " RED "[3] Level-order Expected \"%s\", got \"%s\"\n" DEFAULT, exp, g_log);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Unbalanced 4-level BFS (levels & is_first_elem) passed\n" DEFAULT);
	free_tree(root);

	return (error);
}
