#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex01/ft_strncmp.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char			*desc;
	char			*s1;
	char			*s2;
	unsigned int	n;
}	t_test;

int	sign_of(int x);
int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Different strings with n = 0", .s1 = "abc", .s2 = "xyz", .n = 0},
		{.desc = "Empty strings with n = 0", .s1 = "", .s2 = "", .n = 0},
		{.desc = "Empty strings with n = 5", .s1 = "", .s2 = "", .n = 5},
		{.desc = "Non-empty vs empty (n = 3)", .s1 = "Hello", .s2 = "", .n = 3},
		{.desc = "Empty vs non-empty (n = 4)", .s1 = "", .s2 = "World", .n = 4},
		{.desc = "Diff after n (abcdef vs abcxyz, n = 3)", .s1 = "abcdef", .s2 = "abcxyz", .n = 3},
		{.desc = "Diff right at n-1 (abcdef vs abcxyz, n = 4)", .s1 = "abcdef", .s2 = "abcxyz", .n = 4},
		{.desc = "n larger than both strings (equal)", .s1 = "Hello", .s2 = "Hello", .n = 20},
		{.desc = "n larger than shorter string (Hello vs Hello1)", .s1 = "Hello", .s2 = "Hello1", .n = 10},
		{.desc = "Stops at NUL even with large n", .s1 = "ab\0x", .s2 = "ab\0y", .n = 4},
		{.desc = "Unsigned char comparison (0x80 > 0x7f, n = 1)", .s1 = "\x80", .s2 = "\x7f", .n = 1},
		{.desc = "Unsigned char comparison (0xff > 0x01, n = 5)", .s1 = "a\xff", .s2 = "a\x01", .n = 5},
		{.desc = "Unsigned char diff after n (n = 1)", .s1 = "a\xff", .s2 = "a\x01", .n = 1},
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
		int	expected = strncmp(tests[i].s1, tests[i].s2, tests[i].n);
		int	result = ft_strncmp(tests[i].s1, tests[i].s2, tests[i].n);

		if (sign_of(result) != sign_of(expected))
		{
			printf("    " RED "[%d] %s Expected sign %d (strncmp=%d), got %d\n" DEFAULT,
				i + 1, tests[i].desc, sign_of(expected), expected, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (got %d)\n" DEFAULT,
				i + 1, tests[i].desc, result);
	}
	return (error);
}
