#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_push_back(t_list **begin_list, void *data);

t_list	*ft_create_elem(void *data)
{
	t_list	*elem = malloc(sizeof(t_list));
	if (!elem)
		return (NULL);
	elem->data = data;
	elem->next = NULL;
	return (elem);
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
	t_list	*list = NULL;

	ft_list_push_back(&list, "first");
	if (!list || strcmp((char *)list->data, "first") != 0 || list->next != NULL)
	{
		printf("    " RED "[1] Push back onto empty list failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Push back onto empty list passed\n" DEFAULT);

	ft_list_push_back(&list, "second");
	ft_list_push_back(&list, "third");
	if (!list || !list->next || !list->next->next || list->next->next->next != NULL
		|| strcmp((char *)list->data, "first") != 0
		|| strcmp((char *)list->next->data, "second") != 0
		|| strcmp((char *)list->next->next->data, "third") != 0)
	{
		printf("    " RED "[2] Multiple push_back order mismatch\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Multiple push_back order (first -> second -> third) passed\n" DEFAULT);

	free_list(list);
	return (error);
}
