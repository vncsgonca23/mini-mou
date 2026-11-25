#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex01/ft_range.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		min;
	int		max;
	int		expect_null;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "min == max (0, 0) -> NULL", .min = 0, .max = 0, .expect_null = 1},
		{.desc = "min > max (5, 0) -> NULL", .min = 5, .max = 0, .expect_null = 1},
		{.desc = "min == max == INT_MAX -> NULL", .min = INT_MAX, .max = INT_MAX, .expect_null = 1},
		{.desc = "min == max == INT_MIN -> NULL", .min = INT_MIN, .max = INT_MIN, .expect_null = 1},
		{.desc = "Single element (42, 43)", .min = 42, .max = 43, .expect_null = 0},
		{.desc = "Positive range (0, 5)", .min = 0, .max = 5, .expect_null = 0},
		{.desc = "Negative to positive (-5, 5)", .min = -5, .max = 5, .expect_null = 0},
		{.desc = "All negative (-10, -5)", .min = -10, .max = -5, .expect_null = 0},
		{.desc = "Near INT_MIN (INT_MIN, INT_MIN + 5)", .min = INT_MIN, .max = INT_MIN + 5, .expect_null = 0},
		{.desc = "Near INT_MAX (INT_MAX - 5, INT_MAX)", .min = INT_MAX - 5, .max = INT_MAX, .expect_null = 0},
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
		int	*result = ft_range(tests[i].min, tests[i].max);

		if (tests[i].expect_null)
		{
			if (result != NULL)
			{
				printf("    " RED "[%d] %s Expected NULL, got %p\n" DEFAULT,
					i + 1, tests[i].desc, (void *)result);
				error -= 1;
				free(result);
			}
			else
				printf("  " GREEN CHECKMARK GREY " [%d] %s Returned NULL as expected\n" DEFAULT,
					i + 1, tests[i].desc);
			continue ;
		}
		if (result == NULL)
		{
			printf("    " RED "[%d] %s Unexpected NULL return\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
			continue ;
		}
		long	size = (long)tests[i].max - (long)tests[i].min;
		int		ok = 1;
		for (long j = 0; j < size; j++)
		{
			int	exp = (int)((long)tests[i].min + j);
			if (result[j] != exp)
			{
				printf("    " RED "[%d] %s Index %ld: expected %d, got %d\n" DEFAULT,
					i + 1, tests[i].desc, j, exp, result[j]);
				ok = 0;
				error -= 1;
				break ;
			}
		}
		if (ok)
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (%ld ints)\n" DEFAULT,
				i + 1, tests[i].desc, size);
		free(result);
	}
	return (error);
}
