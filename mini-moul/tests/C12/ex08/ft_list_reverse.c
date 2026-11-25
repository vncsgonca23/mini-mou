#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_reverse(t_list **begin_list);

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
	int		error = 0;

	/* Test 1: Empty list */
	t_list	*empty = NULL;
	ft_list_reverse(&empty);
	if (empty != NULL)
	{
		printf("    " RED "[1] Empty list reverse failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list reverse passed\n" DEFAULT);

	/* Test 2: Single node */
	t_list	*one = mk("only", NULL);
	t_list	*orig_one = one;
	ft_list_reverse(&one);
	if (one != orig_one || one->next != NULL)
	{
		printf("    " RED "[2] Single node reverse failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single node reverse passed\n" DEFAULT);
	free_list(one);

	/* Test 3: 4 nodes (verify actual node links reversed, not just data) */
	t_list	*n3 = mk("3", NULL);
	t_list	*n2 = mk("2", n3);
	t_list	*n1 = mk("1", n2);
	t_list	*n0 = mk("0", n1);
	t_list	*head = n0;
	ft_list_reverse(&head);
	if (head != n3 || head->next != n2 || head->next->next != n1
		|| head->next->next->next != n0 || n0->next != NULL)
	{
		printf("    " RED "[3] 4-node pointer reversal failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] 4-node reversal (links & data preserved) passed\n" DEFAULT);
	free_list(head);

	return (error);
}
