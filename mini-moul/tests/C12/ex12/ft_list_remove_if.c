#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

static int	g_freed;

static void	counted_free(void *ptr)
{
	g_freed++;
	free(ptr);
}

static int	cmp_str(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b));
}

static t_list	*mk(const char *s, t_list *next)
{
	t_list	*n = malloc(sizeof(t_list));
	n->data = strdup(s);
	n->next = next;
	return (n);
}

static void	free_list(t_list *h)
{
	while (h)
	{
		t_list	*t = h->next;
		free(h->data);
		free(h);
		h = t;
	}
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Empty list */
	t_list	*empty = NULL;
	g_freed = 0;
	ft_list_remove_if(&empty, "x", &cmp_str, &counted_free);
	if (empty != NULL || g_freed != 0)
	{
		printf("    " RED "[1] Empty list failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list passed\n" DEFAULT);

	/* Test 2: Remove consecutive heads, middles, and tail */
	t_list	*l = mk("x", mk("x", mk("a", mk("x", mk("b", mk("x", NULL))))));
	g_freed = 0;
	ft_list_remove_if(&l, "x", &cmp_str, &counted_free);
	if (g_freed != 4 || !l || !l->next || l->next->next != NULL
		|| strcmp((char *)l->data, "a") != 0
		|| strcmp((char *)l->next->data, "b") != 0)
	{
		printf("    " RED "[2] Remove head/middle/tail failed (freed=%d)\n" DEFAULT, g_freed);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Removed consecutive head, middle, and tail matches (freed=4)\n" DEFAULT);
	free_list(l);

	/* Test 3: Remove ALL elements -> *begin_list == NULL */
	t_list	*all = mk("x", mk("x", NULL));
	g_freed = 0;
	ft_list_remove_if(&all, "x", &cmp_str, &counted_free);
	if (all != NULL || g_freed != 2)
	{
		printf("    " RED "[3] Remove all elements failed (all=%p, freed=%d)\n" DEFAULT, (void *)all, g_freed);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Removing all elements left list NULL (freed=2)\n" DEFAULT);

	return (error);
}
