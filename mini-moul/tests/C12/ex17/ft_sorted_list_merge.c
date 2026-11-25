#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_sorted_list_merge(t_list **begin_list1, t_list *begin_list2, int (*cmp)());

static int	cmp_str(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b));
}

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

static int	check_order(t_list *h, char **exp, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (!h || strcmp((char *)h->data, exp[i]) != 0)
			return (0);
		h = h->next;
	}
	return (h == NULL);
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Merge into empty list1 */
	t_list	*l1 = NULL;
	t_list	*l2 = mk("a", mk("c", NULL));
	ft_sorted_list_merge(&l1, l2, &cmp_str);
	char	*e1[] = {"a", "c"};
	if (!check_order(l1, e1, 2))
	{
		printf("    " RED "[1] Merge into empty list1 failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Merge into empty list1 passed\n" DEFAULT);
	free_list(l1);

	/* Test 2: Merge empty list2 into non-empty list1 */
	l1 = mk("b", mk("d", NULL));
	ft_sorted_list_merge(&l1, NULL, &cmp_str);
	char	*e2[] = {"b", "d"};
	if (!check_order(l1, e2, 2))
	{
		printf("    " RED "[2] Merge NULL list2 failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Merge NULL list2 passed\n" DEFAULT);

	/* Test 3: Interleaved merge with duplicates and new head */
	l2 = mk("a", mk("b", mk("c", mk("z", NULL))));
	ft_sorted_list_merge(&l1, l2, &cmp_str);
	char	*e3[] = {"a", "b", "b", "c", "d", "z"};
	if (!check_order(l1, e3, 6))
	{
		printf("    " RED "[3] Interleaved sorted merge failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Interleaved sorted merge (with new head & duplicates) passed\n" DEFAULT);
	free_list(l1);

	return (error);
}
