#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_merge(t_list **begin_list1, t_list *begin_list2);

static t_list	*mk(void *data, t_list *next)
{
	t_list	*n = malloc(sizeof(t_list));
	n->data = data;
	n->next = next;
	return (n);
}

static void	free_list(t_list *h)
{
	while (h)
	{
		t_list	*t = h->next;
		free(h);
		h = t;
	}
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Empty list1 + non-empty list2 */
	t_list	*l1 = NULL;
	t_list	*l2 = mk("a", mk("b", NULL));
	ft_list_merge(&l1, l2);
	if (l1 != l2 || strcmp((char *)l1->data, "a") != 0 || strcmp((char *)l1->next->data, "b") != 0)
	{
		printf("    " RED "[1] Merge into empty list1 failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Merge into empty list1 passed\n" DEFAULT);
	free_list(l1);

	/* Test 2: Non-empty list1 + empty list2 */
	l1 = mk("1", mk("2", NULL));
	ft_list_merge(&l1, NULL);
	if (!l1 || !l1->next || l1->next->next != NULL)
	{
		printf("    " RED "[2] Merge with NULL list2 failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Merge with NULL list2 passed\n" DEFAULT);

	/* Test 3: Non-empty list1 + non-empty list2 (reusing exact nodes) */
	t_list	*b2 = mk("3", mk("4", NULL));
	ft_list_merge(&l1, b2);
	if (!l1 || !l1->next || l1->next->next != b2 || !b2->next || b2->next->next != NULL)
	{
		printf("    " RED "[3] Merge two non-empty lists failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Merge two non-empty lists (exact node links) passed\n" DEFAULT);
	free_list(l1);

	return (error);
}
