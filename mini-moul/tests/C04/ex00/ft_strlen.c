#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex00/ft_strlen.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*input;
	int		expected_output;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Empty string", .input = "", .expected_output = 0},
		{.desc = "Single character", .input = "H", .expected_output = 1},
		{.desc = "Standard string", .input = "Hello, world!", .expected_output = 13},
		{.desc = "String with embedded null character", .input = "Hello\0, world!", .expected_output = 5},
		{.desc = "Whitespace and newlines", .input = " \t\n\r\v\f", .expected_output = 6},
		{.desc = "Non-ASCII bytes", .input = "\x80\xFE\xFF", .expected_output = 3},
		{.desc = "Very long string", .input = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", .expected_output = 62},
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
		int	result = ft_strlen(tests[i].input);

		if (result != tests[i].expected_output)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_output, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output %d as expected\n" DEFAULT,
				i + 1, tests[i].desc, result);
	}
	return (error);
}
