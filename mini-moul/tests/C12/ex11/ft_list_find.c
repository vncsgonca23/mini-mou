#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

t_list	*ft_list_find(t_list *begin_list, void *data_ref, int (*cmp)());

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

int	main(void)
{
	int		error = 0;
	t_list	*n3 = mk("dup", NULL);
	t_list	*n2 = mk("target", n3);
	t_list	*n1 = mk("dup", n2);
	t_list	*n0 = mk("head", n1);

	if (ft_list_find(NULL, "head", &cmp_str) != NULL)
	{
		printf("    " RED "[1] Empty list expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list -> NULL\n" DEFAULT);

	if (ft_list_find(n0, "dup", &cmp_str) != n1)
	{
		printf("    " RED "[2] Should return FIRST matching node\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Returned first matching node on duplicates\n" DEFAULT);

	if (ft_list_find(n0, "target", &cmp_str) != n2)
	{
		printf("    " RED "[3] Failed to find middle element\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Found middle element\n" DEFAULT);

	if (ft_list_find(n0, "absent", &cmp_str) != NULL)
	{
		printf("    " RED "[4] Absent element expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] Absent element -> NULL\n" DEFAULT);

	free_list(n0);
	return (error);
}
