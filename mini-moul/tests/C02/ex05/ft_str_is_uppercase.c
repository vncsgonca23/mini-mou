#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex05/ft_str_is_uppercase.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*str;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Empty string", .str = "", .expected = 1},
		{.desc = "Full uppercase alphabet", .str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .expected = 1},
		{.desc = "Contains lowercase letter", .str = "ABCDEFGHaI", .expected = 0},
		{.desc = "Char before 'A' ('@')", .str = "ABC@DEF", .expected = 0},
		{.desc = "Char after 'Z' ('[')", .str = "ABC[DEF", .expected = 0},
		{.desc = "Contains digits", .str = "ABC123", .expected = 0},
		{.desc = "Contains space", .str = "HELLO WORLD", .expected = 0},
		{.desc = "Contains symbols and lowercase", .str = "-_134556ABCDEFGaH67", .expected = 0},
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
		int	result = ft_str_is_uppercase(tests[i].str);

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
