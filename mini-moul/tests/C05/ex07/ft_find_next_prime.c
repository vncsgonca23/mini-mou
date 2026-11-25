#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex07/ft_find_next_prime.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		n;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Next prime >= INT_MIN", .n = INT_MIN, .expected = 2},
		{.desc = "Next prime >= -5", .n = -5, .expected = 2},
		{.desc = "Next prime >= 0", .n = 0, .expected = 2},
		{.desc = "Next prime >= 1", .n = 1, .expected = 2},
		{.desc = "Next prime >= 2", .n = 2, .expected = 2},
		{.desc = "Next prime >= 3", .n = 3, .expected = 3},
		{.desc = "Next prime >= 4", .n = 4, .expected = 5},
		{.desc = "Next prime >= 8", .n = 8, .expected = 11},
		{.desc = "Next prime >= 10", .n = 10, .expected = 11},
		{.desc = "Next prime >= 11", .n = 11, .expected = 11},
		{.desc = "Next prime >= 14", .n = 14, .expected = 17},
		{.desc = "Next prime >= 90", .n = 90, .expected = 97},
		{.desc = "Next prime >= 2147483629", .n = 2147483629, .expected = 2147483629},
		{.desc = "Next prime >= 2147483630", .n = 2147483630, .expected = 2147483647},
		{.desc = "Next prime >= 2147483646", .n = 2147483646, .expected = 2147483647},
		{.desc = "Next prime >= INT_MAX (2147483647)", .n = INT_MAX, .expected = 2147483647},
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
		int	result = ft_find_next_prime(tests[i].n);

		if (result != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, result);
	}
	return (error);
}
