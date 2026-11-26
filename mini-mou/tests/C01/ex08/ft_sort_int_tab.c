#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex08/ft_sort_int_tab.c"
#include "../../../utils/constants.h"

#define CANARY 0x5A5A5A5A

typedef struct s_test
{
	char	*desc;
	int		size;
	int		input[16];
	int		expected[16];
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "Empty array (size = 0)",
			.size = 0,
			.input = {0},
			.expected = {0},
		},
		{
			.desc = "Single element array (size = 1)",
			.size = 1,
			.input = {42},
			.expected = {42},
		},
		{
			.desc = "Two elements unsorted",
			.size = 2,
			.input = {9, -1},
			.expected = {-1, 9},
		},
		{
			.desc = "Two elements already sorted",
			.size = 2,
			.input = {-1, 9},
			.expected = {-1, 9},
		},
		{
			.desc = "Standard unsorted array",
			.size = 6,
			.input = {7, 6, 3, 4, 2, 5},
			.expected = {2, 3, 4, 5, 6, 7},
		},
		{
			.desc = "Already sorted array",
			.size = 5,
			.input = {1, 2, 3, 4, 5},
			.expected = {1, 2, 3, 4, 5},
		},
		{
			.desc = "Reverse sorted array",
			.size = 6,
			.input = {6, 5, 4, 3, 2, 1},
			.expected = {1, 2, 3, 4, 5, 6},
		},
		{
			.desc = "Array with duplicates",
			.size = 7,
			.input = {3, 1, 2, 1, 3, 0, 2},
			.expected = {0, 1, 1, 2, 2, 3, 3},
		},
		{
			.desc = "Array with all identical values",
			.size = 5,
			.input = {42, 42, 42, 42, 42},
			.expected = {42, 42, 42, 42, 42},
		},
		{
			.desc = "Negative values and INT_MIN/INT_MAX",
			.size = 6,
			.input = {INT_MAX, -1, 0, INT_MIN, 42, -42},
			.expected = {INT_MIN, -42, -1, 0, 42, INT_MAX},
		},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

int	run_tests(t_test *tests, int count)
{
	int	i;
	int	error = 0;

	for (i = 0; i < count; i++)
	{
		int	buf[18];
		int	size = tests[i].size;
		int	ok = 1;

		buf[0] = CANARY;
		for (int j = 0; j < size; j++)
			buf[j + 1] = tests[i].input[j];
		buf[size + 1] = CANARY;

		ft_sort_int_tab(buf + 1, size);

		if (buf[0] != CANARY || buf[size + 1] != CANARY)
		{
			printf("    " RED "[%d] %s Out-of-bounds memory write detected\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
			continue ;
		}
		for (int j = 0; j < size; j++)
		{
			if (buf[j + 1] != tests[i].expected[j])
				ok = 0;
		}
		if (!ok)
		{
			printf("    " RED "[%d] %s Failed to sort tab\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT,
				i + 1, tests[i].desc);
	}
	return (error);
}
