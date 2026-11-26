#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_clear(t_list *begin_list, void (*free_fct)(void *));

static int	g_freed_count;

static void	counted_free(void *ptr)
{
	g_freed_count++;
	free(ptr);
}

static t_list	*mk(const char *s, t_list *next)
{
	t_list	*n = malloc(sizeof(t_list));
	n->data = strdup(s);
	n->next = next;
	return (n);
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Empty list */
	g_freed_count = 0;
	ft_list_clear(NULL, &counted_free);
	if (g_freed_count != 0)
	{
		printf("    " RED "[1] Empty list called free_fct %d times\n" DEFAULT, g_freed_count);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list handled cleanly\n" DEFAULT);

	/* Test 2: 4-node list */
	t_list	*list = mk("a", mk("b", mk("c", mk("d", NULL))));
	g_freed_count = 0;
	ft_list_clear(list, &counted_free);
	if (g_freed_count != 4)
	{
		printf("    " RED "[2] 4-element list called free_fct %d times (expected 4)\n" DEFAULT, g_freed_count);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] 4-element list freed all 4 data items & nodes\n" DEFAULT);

	return (error);
}
