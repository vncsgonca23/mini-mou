#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex03/ft_recursive_power.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		base;
	int		power;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "0^0 == 1 (by subject definition)", .base = 0, .power = 0, .expected = 1},
		{.desc = "0^1 == 0", .base = 0, .power = 1, .expected = 0},
		{.desc = "0^5 == 0", .base = 0, .power = 5, .expected = 0},
		{.desc = "0^-1 == 0", .base = 0, .power = -1, .expected = 0},
		{.desc = "10^0 == 1", .base = 10, .power = 0, .expected = 1},
		{.desc = "(-5)^0 == 1", .base = -5, .power = 0, .expected = 1},
		{.desc = "2^1 == 2", .base = 2, .power = 1, .expected = 2},
		{.desc = "3^2 == 9", .base = 3, .power = 2, .expected = 9},
		{.desc = "(-2)^2 == 4 (even power)", .base = -2, .power = 2, .expected = 4},
		{.desc = "(-2)^3 == -8 (odd power)", .base = -2, .power = 3, .expected = -8},
		{.desc = "2^-3 == 0 (negative power)", .base = 2, .power = -3, .expected = 0},
		{.desc = "(-2)^INT_MIN == 0", .base = -2, .power = INT_MIN, .expected = 0},
		{.desc = "1^100 == 1", .base = 1, .power = 100, .expected = 1},
		{.desc = "2^30 == 1073741824", .base = 2, .power = 30, .expected = 1073741824},
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
		int	result = ft_recursive_power(tests[i].base, tests[i].power);

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
