#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

int	ft_list_size(t_list *begin_list);

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

	if (ft_list_size(NULL) != 0)
	{
		printf("    " RED "[1] Empty list Expected 0, got %d\n" DEFAULT, ft_list_size(NULL));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list -> 0\n" DEFAULT);

	t_list	*one = mk("a", NULL);
	if (ft_list_size(one) != 1)
	{
		printf("    " RED "[2] 1-element list Expected 1, got %d\n" DEFAULT, ft_list_size(one));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] 1-element list -> 1\n" DEFAULT);
	free_list(one);

	t_list	*five = mk("1", mk("2", mk("3", mk("4", mk("5", NULL)))));
	if (ft_list_size(five) != 5)
	{
		printf("    " RED "[3] 5-element list Expected 5, got %d\n" DEFAULT, ft_list_size(five));
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] 5-element list -> 5\n" DEFAULT);
	free_list(five);

	return (error);
}
