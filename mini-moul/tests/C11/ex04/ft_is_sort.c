#include <stdio.h>
#include <stdlib.h>
#include "../../../../ex04/ft_is_sort.c"
#include "../../../utils/constants.h"

static int	cmp_int(int a, int b)
{
	if (a < b)
		return (-1);
	if (a > b)
		return (1);
	return (0);
}

typedef struct s_test
{
	char	*desc;
	int		*tab;
	int		length;
	int		expected;
}	t_test;

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Empty array (length = 0) -> 1", .tab = (int[]){0}, .length = 0, .expected = 1},
		{.desc = "Single element -> 1", .tab = (int[]){42}, .length = 1, .expected = 1},
		{.desc = "Two equal elements -> 1", .tab = (int[]){5, 5}, .length = 2, .expected = 1},
		{.desc = "Strictly ascending -> 1", .tab = (int[]){-10, -2, 0, 5, 42}, .length = 5, .expected = 1},
		{.desc = "Ascending with duplicates -> 1", .tab = (int[]){1, 1, 2, 3, 3, 5}, .length = 6, .expected = 1},
		{.desc = "Strictly descending -> 1", .tab = (int[]){42, 10, 0, -5, -20}, .length = 5, .expected = 1},
		{.desc = "Descending with duplicates -> 1", .tab = (int[]){5, 5, 3, 2, 2, 1}, .length = 6, .expected = 1},
		{.desc = "All equal elements -> 1", .tab = (int[]){7, 7, 7, 7}, .length = 4, .expected = 1},
		{.desc = "Peak in middle (unsorted) -> 0", .tab = (int[]){1, 3, 5, 4, 2}, .length = 5, .expected = 0},
		{.desc = "Valley in middle (unsorted) -> 0", .tab = (int[]){5, 3, 1, 2, 4}, .length = 5, .expected = 0},
		{.desc = "Starts equal then up then down -> 0", .tab = (int[]){2, 2, 3, 1}, .length = 4, .expected = 0},
		{.desc = "Starts equal then down then up -> 0", .tab = (int[]){2, 2, 1, 3}, .length = 4, .expected = 0},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);
	int		error = 0;

	for (int i = 0; i < count; i++)
	{
		int	res = ft_is_sort(tests[i].tab, tests[i].length, &cmp_int);
		if (res != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, res);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, res);
	}
	return (error);
}
