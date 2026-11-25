#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex02/ft_str_is_alpha.c"
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
		{.desc = "Lowercase alphabet", .str = "abcdefghijklmnopqrstuvwxyz", .expected = 1},
		{.desc = "Uppercase alphabet", .str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .expected = 1},
		{.desc = "Mixed case letters", .str = "HelloWorldAzZa", .expected = 1},
		{.desc = "Char before 'A' ('@')", .str = "abc@def", .expected = 0},
		{.desc = "Chars between 'Z' and 'a' ('[', '`')", .str = "abcghij[`aBCZ", .expected = 0},
		{.desc = "Char after 'z' ('{')", .str = "abc{def", .expected = 0},
		{.desc = "Contains digits", .str = "23adsaffq", .expected = 0},
		{.desc = "Contains space", .str = "Hello World", .expected = 0},
		{.desc = "Contains non-ASCII byte", .str = "abc\x80", .expected = 0},
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
		int	result = ft_str_is_alpha(tests[i].str);

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
