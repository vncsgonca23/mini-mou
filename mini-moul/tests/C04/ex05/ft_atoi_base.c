#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../../../../ex05/ft_atoi_base.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*str;
	char	*base;
	int		expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Binary positive (101101)", .str = "101101", .base = "01", .expected = 45},
		{.desc = "Hexadecimal lowercase (ff)", .str = "ff", .base = "0123456789abcdef", .expected = 255},
		{.desc = "Decimal with spaces and +-", .str = "    +-14353", .base = "0123456789", .expected = -14353},
		{.desc = "Binary with spaces and ---", .str = "         ---10101001", .base = "01", .expected = -169},
		{.desc = "Hex with all isspace(3) and +---", .str = " \t\n\v\f\r+---59xyz", .base = "0123456789abcdef", .expected = -89},
		{.desc = "Custom octal base poneyvif (vp)", .str = "vp", .base = "poneyvif", .expected = 40},
		{.desc = "Zero in custom base", .str = "ppp", .base = "poneyvif", .expected = 0},
		{.desc = "INT_MAX in decimal", .str = "2147483647", .base = "0123456789", .expected = INT_MAX},
		{.desc = "INT_MIN in decimal", .str = "-2147483648", .base = "0123456789", .expected = INT_MIN},
		{.desc = "INT_MIN in hex", .str = "-80000000", .base = "0123456789ABCDEF", .expected = INT_MIN},
		{.desc = "INT_MIN in binary", .str = "-10000000000000000000000000000000", .base = "01", .expected = INT_MIN},
		{.desc = "Stops at non-base char", .str = "1012101", .base = "01", .expected = 5},
		{.desc = "Space between sign and digits", .str = "- 101", .base = "01", .expected = 0},
		{.desc = "Empty input string", .str = "", .base = "0123456789", .expected = 0},
		{.desc = "Invalid: empty base", .str = "42", .base = "", .expected = 0},
		{.desc = "Invalid: single char base", .str = "000", .base = "0", .expected = 0},
		{.desc = "Invalid: duplicate char in base", .str = "42", .base = "0123456780", .expected = 0},
		{.desc = "Invalid: base contains '+'", .str = "42", .base = "012+456", .expected = 0},
		{.desc = "Invalid: base contains '-'", .str = "42", .base = "012-456", .expected = 0},
		{.desc = "Invalid: base contains space", .str = "42", .base = "012 456", .expected = 0},
		{.desc = "Invalid: base contains tab", .str = "42", .base = "012\t456", .expected = 0},
		{.desc = "Invalid: base contains newline", .str = "42", .base = "012\n456", .expected = 0},
		{.desc = "Invalid: base contains \\v/\\f/\\r", .str = "42", .base = "012\v456", .expected = 0},
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
		int	result = ft_atoi_base(tests[i].str, tests[i].base);

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
