#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex03/ft_atoi.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*input;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Subject example (   ---+--+1234ab567)", .input = "   ---+--+1234ab567", .expected = -1234},
		{.desc = "Positive integer", .input = "12345", .expected = 12345},
		{.desc = "Negative integer", .input = "-54321", .expected = -54321},
		{.desc = "Leading plus sign", .input = "+6789", .expected = 6789},
		{.desc = "All isspace(3) chars ( \\t\\n\\v\\f\\r)", .input = " \t\n\v\f\r42", .expected = 42},
		{.desc = "Even number of minus signs", .input = "--42", .expected = 42},
		{.desc = "Odd number of minus signs with pluses", .input = "+-+--42", .expected = -42},
		{.desc = "Non-numeric suffix", .input = "123abc456", .expected = 123},
		{.desc = "Positive zero", .input = "0", .expected = 0},
		{.desc = "Negative zero", .input = "-0", .expected = 0},
		{.desc = "Leading zeros", .input = "+000123", .expected = 123},
		{.desc = "Empty string", .input = "", .expected = 0},
		{.desc = "Only whitespace", .input = " \t\n\v\f\r", .expected = 0},
		{.desc = "Only signs", .input = "---+++", .expected = 0},
		{.desc = "Space between sign and digits", .input = "- 42", .expected = 0},
		{.desc = "Letter between sign and digits", .input = "-a42", .expected = 0},
		{.desc = "Non-numeric prefix", .input = "abc123", .expected = 0},
		{.desc = "INT_MAX (2147483647)", .input = "2147483647", .expected = INT_MAX},
		{.desc = "INT_MIN (-2147483648)", .input = "-2147483648", .expected = INT_MIN},
		{.desc = "INT_MIN with whitespace and multiple signs", .input = "\t\n ---2147483648xyz", .expected = INT_MIN},
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
		int	result = ft_atoi(tests[i].input);

		if (result != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output %d as expected\n" DEFAULT,
				i + 1, tests[i].desc, result);
	}
	return (error);
}
