#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex07/ft_advanced_sort_string_tab.c"
#include "../../../utils/constants.h"

static int	cmp_asc(char *a, char *b)
{
	return (strcmp(a, b));
}

static int	cmp_desc(char *a, char *b)
{
	return (strcmp(b, a));
}

static int	cmp_len(char *a, char *b)
{
	return ((int)strlen(a) - (int)strlen(b));
}

static int	check_tab(int num, const char *desc, char **tab, char **expected, int (*cmp)(char *, char *))
{
	ft_advanced_sort_string_tab(tab, cmp);
	int	i = 0;
	while (expected[i] && tab[i])
	{
		if (strcmp(tab[i], expected[i]) != 0)
		{
			printf("    " RED "[%d] %s Index %d: expected \"%s\", got \"%s\"\n" DEFAULT,
				num, desc, i, expected[i], tab[i]);
			return (-1);
		}
		i++;
	}
	if (tab[i] != NULL || expected[i] != NULL)
	{
		printf("    " RED "[%d] %s NULL terminator mismatch at index %d\n" DEFAULT, num, desc, i);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, num, desc);
	return (0);
}

int	main(void)
{
	int	error = 0;

	char	*t1[] = {NULL};
	char	*e1[] = {NULL};
	error += check_tab(1, "Empty tab", t1, e1, &cmp_asc);

	char	*t2[] = {"only", NULL};
	char	*e2[] = {"only", NULL};
	error += check_tab(2, "Single element tab", t2, e2, &cmp_asc);

	char	*t3[] = {"charlie", "alpha", "delta", "bravo", NULL};
	char	*e3[] = {"alpha", "bravo", "charlie", "delta", NULL};
	error += check_tab(3, "Ascending ASCII comparator", t3, e3, &cmp_asc);

	char	*t4[] = {"charlie", "alpha", "delta", "bravo", NULL};
	char	*e4[] = {"delta", "charlie", "bravo", "alpha", NULL};
	error += check_tab(4, "Descending ASCII comparator", t4, e4, &cmp_desc);

	char	*t5[] = {"1234", "1", "123", "12", "", NULL};
	char	*e5[] = {"", "1", "12", "123", "1234", NULL};
	error += check_tab(5, "Sort by string length comparator", t5, e5, &cmp_len);

	return (error);
}
