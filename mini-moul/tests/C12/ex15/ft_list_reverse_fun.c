#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_reverse_fun(t_list *begin_list);

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

	/* Test 1: Empty list (must not crash) */
	ft_list_reverse_fun(NULL);
	printf("  " GREEN CHECKMARK GREY " [1] Empty list handled cleanly\n" DEFAULT);

	/* Test 2: Single node */
	t_list	*one = mk("only", NULL);
	ft_list_reverse_fun(one);
	char	*e1[] = {"only"};
	if (!check_order(one, e1, 1))
	{
		printf("    " RED "[2] Single node reverse_fun failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single node reverse_fun passed\n" DEFAULT);
	free_list(one);

	/* Test 3: Even count (4 nodes) */
	t_list	*even = mk("1", mk("2", mk("3", mk("4", NULL))));
	ft_list_reverse_fun(even);
	char	*e4[] = {"4", "3", "2", "1"};
	if (!check_order(even, e4, 4))
	{
		printf("    " RED "[3] Even 4-node reverse_fun failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Even 4-node reverse_fun passed\n" DEFAULT);
	free_list(even);

	/* Test 4: Odd count (5 nodes) */
	t_list	*odd = mk("a", mk("b", mk("c", mk("d", mk("e", NULL)))));
	ft_list_reverse_fun(odd);
	char	*e5[] = {"e", "d", "c", "b", "a"};
	if (!check_order(odd, e5, 5))
	{
		printf("    " RED "[4] Odd 5-node reverse_fun failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] Odd 5-node reverse_fun passed\n" DEFAULT);
	free_list(odd);

	return (error);
}
