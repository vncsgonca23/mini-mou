#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

void	ft_list_foreach(t_list *begin_list, void (*f)(void *));

static char	g_buf[64];

static void	append_str(void *data)
{
	strcat(g_buf, (char *)data);
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
	int	error = 0;

	g_buf[0] = '\0';
	ft_list_foreach(NULL, &append_str);
	if (g_buf[0] != '\0')
	{
		printf("    " RED "[1] Empty list invoked f\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Empty list handled cleanly\n" DEFAULT);

	t_list	*list = mk("A", mk("B", mk("C", mk("D", NULL))));
	g_buf[0] = '\0';
	ft_list_foreach(list, &append_str);
	if (strcmp(g_buf, "ABCD") != 0)
	{
		printf("    " RED "[2] Expected \"ABCD\", got \"%s\"\n" DEFAULT, g_buf);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Applied f in exact list order (\"ABCD\")\n" DEFAULT);
	free_list(list);

	return (error);
}
