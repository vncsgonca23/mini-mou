#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex00/ft_strcmp.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*s1;
	char	*s2;
}	t_test;

int	sign_of(int x);
int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Equal strings", .s1 = "Hello", .s2 = "Hello"},
		{.desc = "Empty strings", .s1 = "", .s2 = ""},
		{.desc = "Non-empty vs empty", .s1 = "Hello", .s2 = ""},
		{.desc = "Empty vs non-empty", .s1 = "", .s2 = "Hello"},
		{.desc = "Prefix longer (Hello vs He)", .s1 = "Hello", .s2 = "He"},
		{.desc = "Prefix shorter (Hello vs Hello1)", .s1 = "Hello", .s2 = "Hello1"},
		{.desc = "Single char a < b", .s1 = "a", .s2 = "b"},
		{.desc = "Single char b > a", .s1 = "b", .s2 = "a"},
		{.desc = "Case difference (A < a)", .s1 = "Hello", .s2 = "hello"},
		{.desc = "Diff at last char (ab < ac)", .s1 = "ab", .s2 = "ac"},
		{.desc = "Unsigned char comparison (0x80 > 0x7f)", .s1 = "\x80", .s2 = "\x7f"},
		{.desc = "Unsigned char comparison (0xff > 0x01)", .s1 = "\xff", .s2 = "\x01"},
		{.desc = "Unsigned char comparison (0x01 < 0xff)", .s1 = "\x01", .s2 = "\xff"},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

int	sign_of(int x)
{
	if (x < 0)
		return (-1);
	if (x > 0)
		return (1);
	return (0);
}

int	run_tests(t_test *tests, int count)
{
	int	i;
	int	error = 0;

	for (i = 0; i < count; i++)
	{
		int	expected = strcmp(tests[i].s1, tests[i].s2);
		int	result = ft_strcmp(tests[i].s1, tests[i].s2);

		if (sign_of(result) != sign_of(expected))
		{
			printf("    " RED "[%d] %s Expected sign %d (strcmp=%d), got %d\n" DEFAULT,
				i + 1, tests[i].desc, sign_of(expected), expected, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (got %d)\n" DEFAULT,
				i + 1, tests[i].desc, result);
	}
	return (error);
}
