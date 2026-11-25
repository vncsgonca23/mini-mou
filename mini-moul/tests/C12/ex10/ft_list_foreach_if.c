#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_foreach_if(t_list *begin_list, void (*f)(void *), void *data_ref, int (*cmp)());

static int	g_hits;

static void	mark_upper(void *data)
{
	char	*s = (char *)data;
	s[0] = 'X';
	g_hits++;
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

	g_hits = 0;
	ft_list_foreach_if(NULL, &mark_upper, "hit", &cmp_str);
	if (g_hits != 0)
	{
		printf("    " RED "[1] Empty list invoked f\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list handled cleanly\n" DEFAULT);

	t_list	*list = mk("hit", mk("keep", mk("hit", mk("stay", NULL))));
	g_hits = 0;
	ft_list_foreach_if(list, &mark_upper, "hit", &cmp_str);
	if (g_hits != 2
		|| strcmp((char *)list->data, "Xit") != 0
		|| strcmp((char *)list->next->data, "keep") != 0
		|| strcmp((char *)list->next->next->data, "Xit") != 0
		|| strcmp((char *)list->next->next->next->data, "stay") != 0)
	{
		printf("    " RED "[2] Conditional foreach mismatch (hits=%d)\n" DEFAULT, g_hits);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Applied f only to matching elements (2/4)\n" DEFAULT);
	free_list(list);

	return (error);
}
