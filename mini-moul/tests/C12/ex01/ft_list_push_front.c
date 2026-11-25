#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_push_front(t_list **begin_list, void *data);

t_list	*ft_create_elem(void *data)
{
	t_list	*elem = malloc(sizeof(t_list));
	if (!elem)
		return (NULL);
	elem->data = data;
	elem->next = NULL;
	return (elem);
}

static void	free_list(t_list *head)
{
	while (head)
	{
		t_list	*tmp = head->next;
		free(head);
		head = tmp;
	}
}

int	main(void)
{
	int		error = 0;
	t_list	*list = NULL;

	/* Test 1: Push onto empty list */
	ft_list_push_front(&list, "first");
	if (!list || strcmp((char *)list->data, "first") != 0 || list->next != NULL)
	{
		printf("    " RED "[1] Push onto empty list failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Push onto empty list passed\n" DEFAULT);

	/* Test 2: Push multiple elements in order -> LIFO head */
	ft_list_push_front(&list, "second");
	ft_list_push_front(&list, "third");
	if (!list || !list->next || !list->next->next || list->next->next->next != NULL
		|| strcmp((char *)list->data, "third") != 0
		|| strcmp((char *)list->next->data, "second") != 0
		|| strcmp((char *)list->next->next->data, "first") != 0)
	{
		printf("    " RED "[2] Multiple push_front order mismatch\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Multiple push_front order (third -> second -> first) passed\n" DEFAULT);

	/* Test 3: Push NULL data */
	ft_list_push_front(&list, NULL);
	if (!list || list->data != NULL || strcmp((char *)list->next->data, "third") != 0)
	{
		printf("    " RED "[3] Push NULL data failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Push NULL data passed\n" DEFAULT);

	free_list(list);
	return (error);
}
