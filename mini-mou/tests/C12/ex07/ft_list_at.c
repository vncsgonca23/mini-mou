#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

t_list	*ft_list_at(t_list *begin_list, unsigned int nbr);

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
	t_list	*n2 = mk("two", NULL);
	t_list	*n1 = mk("one", n2);
	t_list	*n0 = mk("zero", n1);

	if (ft_list_at(NULL, 0) != NULL || ft_list_at(NULL, 5) != NULL)
	{
		printf("    " RED "[1] Empty list did not return NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list -> NULL\n" DEFAULT);

	if (ft_list_at(n0, 0) != n0 || ft_list_at(n0, 1) != n1 || ft_list_at(n0, 2) != n2)
	{
		printf("    " RED "[2] Valid indices (0, 1, 2) returned wrong node\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Valid indices (0, 1, 2) returned exact nodes\n" DEFAULT);

	if (ft_list_at(n0, 3) != NULL || ft_list_at(n0, 100) != NULL)
	{
		printf("    " RED "[3] Out-of-bounds indices did not return NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Out-of-bounds indices -> NULL\n" DEFAULT);

	free_list(n0);
	return (error);
}
