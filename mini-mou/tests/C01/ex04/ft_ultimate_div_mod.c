#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex04/ft_ultimate_div_mod.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		a;
	int		b;
	int		expected_a;
	int		expected_b;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_ultimate_div_mod(15, 3)", .a = 15, .b = 3, .expected_a = 5, .expected_b = 0},
		{.desc = "ft_ultimate_div_mod(23, 5)", .a = 23, .b = 5, .expected_a = 4, .expected_b = 3},
		{.desc = "ft_ultimate_div_mod(32, 5)", .a = 32, .b = 5, .expected_a = 6, .expected_b = 2},
		{.desc = "ft_ultimate_div_mod(0, 7)", .a = 0, .b = 7, .expected_a = 0, .expected_b = 0},
		{.desc = "ft_ultimate_div_mod(3, 10)", .a = 3, .b = 10, .expected_a = 0, .expected_b = 3},
		{.desc = "ft_ultimate_div_mod(-23, 5)", .a = -23, .b = 5, .expected_a = -4, .expected_b = -3},
		{.desc = "ft_ultimate_div_mod(23, -5)", .a = 23, .b = -5, .expected_a = -4, .expected_b = 3},
		{.desc = "ft_ultimate_div_mod(-23, -5)", .a = -23, .b = -5, .expected_a = 4, .expected_b = -3},
		{.desc = "ft_ultimate_div_mod(INT_MAX, 1)", .a = INT_MAX, .b = 1, .expected_a = INT_MAX, .expected_b = 0},
		{.desc = "ft_ultimate_div_mod(INT_MIN, 2)", .a = INT_MIN, .b = 2, .expected_a = -1073741824, .expected_b = 0},
		{.desc = "ft_ultimate_div_mod(INT_MAX, INT_MAX)", .a = INT_MAX, .b = INT_MAX, .expected_a = 1, .expected_b = 0},
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
		int	x = tests[i].a;
		int	y = tests[i].b;

		ft_ultimate_div_mod(&x, &y);
		if (x != tests[i].expected_a || y != tests[i].expected_b)
		{
			printf("    " RED "[%d] %s Expected a = %d, b = %d, got a = %d, b = %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_a, tests[i].expected_b, x, y);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s a = %d, b = %d as expected\n" DEFAULT,
				i + 1, tests[i].desc, x, y);
	}
	return (error);
}
