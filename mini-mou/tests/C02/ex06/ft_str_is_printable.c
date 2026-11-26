#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex06/ft_str_is_printable.c"
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
		{.desc = "Space (ASCII 32, lowest printable)", .str = " ", .expected = 1},
		{.desc = "Tilde (ASCII 126, highest printable)", .str = "~", .expected = 1},
		{.desc = "All printable ASCII boundaries", .str = " !\"#$%&'()*+,-./09AZaz~", .expected = 1},
		{.desc = "ASCII 31 (below printable)", .str = "abc\x1f" "def", .expected = 0},
		{.desc = "ASCII 127 (DEL, above printable)", .str = "abc\x7f" "def", .expected = 0},
		{.desc = "Control chars \\n\\t\\v\\f", .str = "\n\t\v\f", .expected = 0},
		{.desc = "Extended byte 0x80", .str = "abc\x80", .expected = 0},
		{.desc = "Extended byte 0xFF", .str = "\xff", .expected = 0},
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
		int	result = ft_str_is_printable(tests[i].str);

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
