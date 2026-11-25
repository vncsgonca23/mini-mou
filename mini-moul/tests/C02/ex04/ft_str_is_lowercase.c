#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex04/ft_str_is_lowercase.c"
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
		{.desc = "Full lowercase alphabet", .str = "abcdefghijklmnopqrstuvwxyz", .expected = 1},
		{.desc = "Contains uppercase letter", .str = "abcAdefghijkl", .expected = 0},
		{.desc = "Char before 'a' ('`')", .str = "abc`def", .expected = 0},
		{.desc = "Char after 'z' ('{')", .str = "abc{def", .expected = 0},
		{.desc = "Contains digits", .str = "abc123", .expected = 0},
		{.desc = "Contains space", .str = "hello world", .expected = 0},
		{.desc = "Contains symbols and uppercase", .str = "-_134556efSghij67", .expected = 0},
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
		int	result = ft_str_is_lowercase(tests[i].str);

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
