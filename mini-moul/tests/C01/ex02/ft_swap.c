#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex02/ft_swap.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		x;
	int		y;
	int		expected_x;
	int		expected_y;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Swap two positive integers", .x = 2, .y = 3, .expected_x = 3, .expected_y = 2},
		{.desc = "Swap a positive and negative integer", .x = 9, .y = -4, .expected_x = -4, .expected_y = 9},
		{.desc = "Swap two negative integers", .x = -32, .y = -54, .expected_x = -54, .expected_y = -32},
		{.desc = "Swap zero and non-zero", .x = 0, .y = 42, .expected_x = 42, .expected_y = 0},
		{.desc = "Swap equal values", .x = 42, .y = 42, .expected_x = 42, .expected_y = 42},
		{.desc = "Swap INT_MAX and INT_MAX - 1 (overflow check)", .x = INT_MAX, .y = INT_MAX - 1, .expected_x = INT_MAX - 1, .expected_y = INT_MAX},
		{.desc = "Swap INT_MIN and INT_MAX", .x = INT_MIN, .y = INT_MAX, .expected_x = INT_MAX, .expected_y = INT_MIN},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);
	int		error = run_tests(tests, count);
	int		same = 42;

	ft_swap(&same, &same);
	if (same != 42)
	{
		printf("    " RED "[%d] Swap same pointer (&x, &x) Expected x = 42, got x = %d\n" DEFAULT,
			count + 1, same);
		error--;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [%d] Swap same pointer (&x, &x) Passed\n" DEFAULT,
			count + 1);
	return (error);
}

int	run_tests(t_test *tests, int count)
{
	int	i;
	int	error = 0;

	for (i = 0; i < count; i++)
	{
		int	x = tests[i].x;
		int	y = tests[i].y;

		ft_swap(&x, &y);
		if (x != tests[i].expected_x || y != tests[i].expected_y)
		{
			printf("    " RED "[%d] %s Expected x = %d, y = %d, got x = %d, y = %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_x, tests[i].expected_y, x, y);
			error--;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
	}
	return (error);
}
