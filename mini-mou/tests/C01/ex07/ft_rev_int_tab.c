#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex07/ft_rev_int_tab.c"
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
			.desc = "Two elements array (size = 2)",
			.size = 2,
			.input = {1, 2},
			.expected = {2, 1},
		},
		{
			.desc = "Odd length array (size = 5)",
			.size = 5,
			.input = {1, 2, 3, 4, 5},
			.expected = {5, 4, 3, 2, 1},
		},
		{
			.desc = "Even length array (size = 6)",
			.size = 6,
			.input = {0, 1, 2, 4, 3, 5},
			.expected = {5, 3, 4, 2, 1, 0},
		},
		{
			.desc = "Negative numbers and INT_MIN/INT_MAX",
			.size = 5,
			.input = {INT_MIN, -42, 0, 42, INT_MAX},
			.expected = {INT_MAX, 42, 0, -42, INT_MIN},
		},
		{
			.desc = "Palindrome array",
			.size = 4,
			.input = {7, 3, 3, 7},
			.expected = {7, 3, 3, 7},
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

		ft_rev_int_tab(buf + 1, size);

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
			printf("    " RED "[%d] %s Array mismatch after reverse\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT,
				i + 1, tests[i].desc);
	}
	return (error);
}
