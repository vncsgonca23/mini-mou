#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex00/ft_ft.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		initial;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_ft(&n) with n = 33", .initial = 33, .expected = 42},
		{.desc = "ft_ft(&n) with n = 0", .initial = 0, .expected = 42},
		{.desc = "ft_ft(&n) with n = -42", .initial = -42, .expected = 42},
		{.desc = "ft_ft(&n) with n = 42", .initial = 42, .expected = 42},
		{.desc = "ft_ft(&n) with n = INT_MIN", .initial = INT_MIN, .expected = 42},
		{.desc = "ft_ft(&n) with n = INT_MAX", .initial = INT_MAX, .expected = 42},
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
		int	number = tests[i].initial;

		ft_ft(&number);
		if (number != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, number);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, number);
	}
	return (error);
}
