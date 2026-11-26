#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex06/ft_sort_string_tab.c"
#include "../../../utils/constants.h"

static int	check_tab(int num, const char *desc, char **tab, char **expected)
{
	ft_sort_string_tab(tab);
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
	error += check_tab(1, "Empty NULL-terminated tab", t1, e1);

	char	*t2[] = {"solo", NULL};
	char	*e2[] = {"solo", NULL};
	error += check_tab(2, "Single string", t2, e2);

	/* String literals verify pointers are swapped (NOT in-place string mutation!) */
	char	*t3[] = {"a", "Z", "2", "10", "A", "z", "", NULL};
	char	*e3[] = {"", "10", "2", "A", "Z", "a", "z", NULL};
	error += check_tab(3, "ASCII order (empty < digits < upper < lower)", t3, e3);

	char	*t4[] = {"abd", "ab", "abc", "ab", NULL};
	char	*e4[] = {"ab", "ab", "abc", "abd", NULL};
	error += check_tab(4, "Duplicates and common prefixes", t4, e4);

	char	*t5[] = {"z", "y", "x", "w", NULL};
	char	*e5[] = {"w", "x", "y", "z", NULL};
	error += check_tab(5, "Reverse sorted tab", t5, e5);

	return (error);
}
