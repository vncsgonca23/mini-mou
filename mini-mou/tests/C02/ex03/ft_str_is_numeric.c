#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex03/ft_str_is_numeric.c"
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
		{.desc = "All digits 0-9", .str = "0123456789", .expected = 1},
		{.desc = "Long numeric string", .str = "01948987493", .expected = 1},
		{.desc = "Char before '0' ('/')", .str = "123/456", .expected = 0},
		{.desc = "Char after '9' (':')", .str = "123:456", .expected = 0},
		{.desc = "Contains letter", .str = "0484882j8995489", .expected = 0},
		{.desc = "Contains minus sign", .str = "-12345", .expected = 0},
		{.desc = "Contains plus sign", .str = "+12345", .expected = 0},
		{.desc = "Contains space", .str = "123 456", .expected = 0},
		{.desc = "Contains symbols", .str = "-_jsdsdc67sdlkc,", .expected = 0},
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
		int	result = ft_str_is_numeric(tests[i].str);

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
