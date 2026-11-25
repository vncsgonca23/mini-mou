#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_sort(t_list **begin_list, int (*cmp)());

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

	/* Test 1: Empty list */
	t_list	*empty = NULL;
	ft_list_sort(&empty, &cmp_str);
	if (empty != NULL)
	{
		printf("    " RED "[1] Empty list sort failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list sort passed\n" DEFAULT);

	/* Test 2: Single node */
	t_list	*one = mk("only", NULL);
	ft_list_sort(&one, &cmp_str);
	char	*e1[] = {"only"};
	if (!check_order(one, e1, 1))
	{
		printf("    " RED "[2] Single node sort failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single node sort passed\n" DEFAULT);
	free_list(one);

	/* Test 3: Unsorted list with duplicates */
	t_list	*l = mk("d", mk("a", mk("c", mk("a", mk("b", NULL)))));
	ft_list_sort(&l, &cmp_str);
	char	*e3[] = {"a", "a", "b", "c", "d"};
	if (!check_order(l, e3, 5))
	{
		printf("    " RED "[3] Unsorted list with duplicates failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Unsorted list with duplicates sorted ascending\n" DEFAULT);
	free_list(l);

	/* Test 4: Reverse sorted list */
	t_list	*rev = mk("5", mk("4", mk("3", mk("2", mk("1", NULL)))));
	ft_list_sort(&rev, &cmp_str);
	char	*e4[] = {"1", "2", "3", "4", "5"};
	if (!check_order(rev, e4, 5))
	{
		printf("    " RED "[4] Reverse sorted list failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] Reverse sorted list passed\n" DEFAULT);
	free_list(rev);

	return (error);
}
