#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_sorted_list_insert(t_list **begin_list, void *data, int (*cmp)());

t_list	*ft_create_elem(void *data)
{
	t_list	*elem = malloc(sizeof(t_list));
	if (!elem)
		return (NULL);
	elem->data = data;
	elem->next = NULL;
	return (elem);
}

static int	cmp_str(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b));
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
	int		error = 0;
	t_list	*list = NULL;

	/* Insert into empty, then head, tail, middle, and duplicate */
	ft_sorted_list_insert(&list, "m", &cmp_str);
	char	*e1[] = {"m"};
	if (!check_order(list, e1, 1))
	{
		printf("    " RED "[1] Insert into empty list failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Insert into empty list passed\n" DEFAULT);

	ft_sorted_list_insert(&list, "a", &cmp_str);
	ft_sorted_list_insert(&list, "z", &cmp_str);
	ft_sorted_list_insert(&list, "g", &cmp_str);
	ft_sorted_list_insert(&list, "m", &cmp_str);
	char	*e2[] = {"a", "g", "m", "m", "z"};
	if (!check_order(list, e2, 5))
	{
		printf("    " RED "[2] Insert at head, middle, duplicate & tail failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Insert at head, middle, duplicate & tail passed\n" DEFAULT);

	free_list(list);
	return (error);
}
