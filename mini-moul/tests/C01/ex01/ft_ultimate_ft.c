#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../../../ex01/ft_ultimate_ft.c"
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
		{.desc = "ft_ultimate_ft(nbr) with n = 21", .initial = 21, .expected = 42},
		{.desc = "ft_ultimate_ft(nbr) with n = 0", .initial = 0, .expected = 42},
		{.desc = "ft_ultimate_ft(nbr) with n = -42", .initial = -42, .expected = 42},
		{.desc = "ft_ultimate_ft(nbr) with n = 42", .initial = 42, .expected = 42},
		{.desc = "ft_ultimate_ft(nbr) with n = INT_MIN", .initial = INT_MIN, .expected = 42},
		{.desc = "ft_ultimate_ft(nbr) with n = INT_MAX", .initial = INT_MAX, .expected = 42},
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
		int			n = tests[i].initial;
		int			*nbr8 = &n;
		int			**nbr7 = &nbr8;
		int			***nbr6 = &nbr7;
		int			****nbr5 = &nbr6;
		int			*****nbr4 = &nbr5;
		int			******nbr3 = &nbr4;
		int			*******nbr2 = &nbr3;
		int			********nbr1 = &nbr2;
		int			*********nbr = &nbr1;

		ft_ultimate_ft(nbr);
		if (n != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, n);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, n);
	}
	return (error);
}
