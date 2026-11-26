#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

t_list	*ft_list_last(t_list *begin_list);

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

	if (ft_list_last(NULL) != NULL)
	{
		printf("    " RED "[1] Empty list Expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list -> NULL\n" DEFAULT);

	t_list	*one = mk("only", NULL);
	if (ft_list_last(one) != one)
	{
		printf("    " RED "[2] Single element list did not return the element\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single element list passed\n" DEFAULT);
	free_list(one);

	t_list	*tail = mk("last", NULL);
	t_list	*head = mk("first", mk("mid", tail));
	if (ft_list_last(head) != tail)
	{
		printf("    " RED "[3] 3-element list did not return tail node\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] 3-element list returned tail node\n" DEFAULT);
	free_list(head);

	return (error);
}
