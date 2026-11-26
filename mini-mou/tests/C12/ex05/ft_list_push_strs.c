#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

t_list	*ft_list_push_strs(int size, char **strs);

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
	char	*strs[] = {"first", "second", "third", "fourth"};

	/* Test 1: size == 0 -> NULL */
	if (ft_list_push_strs(0, strs) != NULL)
	{
		printf("    " RED "[1] size == 0 Expected NULL\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] size == 0 -> NULL\n" DEFAULT);

	/* Test 2: size == 1 */
	t_list	*l1 = ft_list_push_strs(1, strs);
	if (!l1 || l1->data != strs[0] || l1->next != NULL)
	{
		printf("    " RED "[2] size == 1 failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] size == 1 passed\n" DEFAULT);
	free_list(l1);

	/* Test 3: size == 4 (first element must be at the END of the list) */
	t_list	*l4 = ft_list_push_strs(4, strs);
	t_list	*cur = l4;
	int		ok = 1;
	for (int i = 3; i >= 0; i--)
	{
		if (!cur || cur->data != strs[i])
		{
			ok = 0;
			break ;
		}
		cur = cur->next;
	}
	if (!ok || cur != NULL)
	{
		printf("    " RED "[3] size == 4 order mismatch (expected fourth->third->second->first)\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] size == 4 reversed order (first at tail) passed\n" DEFAULT);
	free_list(l4);

	return (error);
}
