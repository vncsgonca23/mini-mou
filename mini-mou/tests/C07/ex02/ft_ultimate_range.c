#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex02/ft_ultimate_range.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		min;
	int		max;
	int		expected_return;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "min == max (0, 0) -> 0 & *range = NULL", .min = 0, .max = 0, .expected_return = 0},
		{.desc = "min > max (10, 5) -> 0 & *range = NULL", .min = 10, .max = 5, .expected_return = 0},
		{.desc = "min == max == INT_MAX -> 0 & *range = NULL", .min = INT_MAX, .max = INT_MAX, .expected_return = 0},
		{.desc = "min == max == INT_MIN -> 0 & *range = NULL", .min = INT_MIN, .max = INT_MIN, .expected_return = 0},
		{.desc = "Single element (42, 43)", .min = 42, .max = 43, .expected_return = 1},
		{.desc = "Positive range (0, 5)", .min = 0, .max = 5, .expected_return = 5},
		{.desc = "Negative to positive (-5, 5)", .min = -5, .max = 5, .expected_return = 10},
		{.desc = "All negative (-10, -5)", .min = -10, .max = -5, .expected_return = 5},
		{.desc = "Near INT_MIN (INT_MIN, INT_MIN + 5)", .min = INT_MIN, .max = INT_MIN + 5, .expected_return = 5},
		{.desc = "Near INT_MAX (INT_MAX - 5, INT_MAX)", .min = INT_MAX - 5, .max = INT_MAX, .expected_return = 5},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

int	run_tests(t_test *tests, int count)
{
	int	i;
	int	error = 0;
	int	sentinel = 12345;

	for (i = 0; i < count; i++)
	{
		int	*result = &sentinel;
		int	range_size = ft_ultimate_range(&result, tests[i].min, tests[i].max);

		if (tests[i].expected_return == 0)
		{
			if (range_size != 0 || result != NULL)
			{
				printf("    " RED "[%d] %s Expected ret=0 & *range=NULL, got ret=%d & *range=%p\n" DEFAULT,
					i + 1, tests[i].desc, range_size, (void *)result);
				error -= 1;
				if (result != NULL && result != &sentinel)
					free(result);
			}
			else
				printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (ret=0, *range=NULL)\n" DEFAULT,
					i + 1, tests[i].desc);
			continue ;
		}
		if (range_size != tests[i].expected_return || result == NULL || result == &sentinel)
		{
			printf("    " RED "[%d] %s Expected ret=%d & non-NULL, got ret=%d & ptr=%p\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_return, range_size, (void *)result);
			error -= 1;
			if (result != NULL && result != &sentinel)
				free(result);
			continue ;
		}
		int	ok = 1;
		for (int j = 0; j < tests[i].expected_return; j++)
		{
			int	exp = (int)((long)tests[i].min + j);
			if (result[j] != exp)
			{
				printf("    " RED "[%d] %s Index %d: expected %d, got %d\n" DEFAULT,
					i + 1, tests[i].desc, j, exp, result[j]);
				ok = 0;
				error -= 1;
				break ;
			}
		}
		if (ok)
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (size=%d)\n" DEFAULT,
				i + 1, tests[i].desc, range_size);
		free(result);
	}
	return (error);
}
