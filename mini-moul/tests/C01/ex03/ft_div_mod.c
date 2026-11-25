#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex03/ft_div_mod.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		a;
	int		b;
	int		expected_div;
	int		expected_mod;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_div_mod(10, 5)", .a = 10, .b = 5, .expected_div = 2, .expected_mod = 0},
		{.desc = "ft_div_mod(25, 6)", .a = 25, .b = 6, .expected_div = 4, .expected_mod = 1},
		{.desc = "ft_div_mod(47, 8)", .a = 47, .b = 8, .expected_div = 5, .expected_mod = 7},
		{.desc = "ft_div_mod(0, 5)", .a = 0, .b = 5, .expected_div = 0, .expected_mod = 0},
		{.desc = "ft_div_mod(3, 10)", .a = 3, .b = 10, .expected_div = 0, .expected_mod = 3},
		{.desc = "ft_div_mod(-25, 6)", .a = -25, .b = 6, .expected_div = -4, .expected_mod = -1},
		{.desc = "ft_div_mod(25, -6)", .a = 25, .b = -6, .expected_div = -4, .expected_mod = 1},
		{.desc = "ft_div_mod(-25, -6)", .a = -25, .b = -6, .expected_div = 4, .expected_mod = -1},
		{.desc = "ft_div_mod(INT_MAX, 1)", .a = INT_MAX, .b = 1, .expected_div = INT_MAX, .expected_mod = 0},
		{.desc = "ft_div_mod(INT_MIN, 2)", .a = INT_MIN, .b = 2, .expected_div = -1073741824, .expected_mod = 0},
		{.desc = "ft_div_mod(INT_MAX, INT_MAX)", .a = INT_MAX, .b = INT_MAX, .expected_div = 1, .expected_mod = 0},
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
		int	div = -999;
		int	mod = -999;

		ft_div_mod(tests[i].a, tests[i].b, &div, &mod);
		if (div != tests[i].expected_div || mod != tests[i].expected_mod)
		{
			printf("    " RED "[%d] %s Expected div = %d, mod = %d, got div = %d, mod = %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_div, tests[i].expected_mod, div, mod);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s div = %d, mod = %d as expected\n" DEFAULT,
				i + 1, tests[i].desc, div, mod);
	}
	return (error);
}
