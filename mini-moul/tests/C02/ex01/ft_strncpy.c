#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex01/ft_strncpy.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char			*desc;
	char			*src;
	unsigned int	n;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "n = 0 (must not touch dest)", .src = "Hello", .n = 0},
		{.desc = "n < strlen(src) (no NUL added)", .src = "Hello", .n = 3},
		{.desc = "n == strlen(src) (no NUL added)", .src = "Hello", .n = 5},
		{.desc = "n > strlen(src) (must pad with NULs)", .src = "Hello", .n = 10},
		{.desc = "Empty src with n = 5 (all NULs)", .src = "", .n = 5},
		{.desc = "Empty src with n = 0", .src = "", .n = 0},
		{.desc = "Single char src with n = 1", .src = "A", .n = 1},
		{.desc = "Single char src with n = 4 (pad 3 NULs)", .src = "A", .n = 4},
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
		char	actual[32];
		char	expected[32];

		memset(actual, 'X', sizeof(actual));
		memset(expected, 'X', sizeof(expected));

		strncpy(expected, tests[i].src, tests[i].n);
		char	*result = ft_strncpy(actual, tests[i].src, tests[i].n);

		if (result != actual)
		{
			printf("    " RED "[%d] %s Did not return dest pointer\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else if (memcmp(actual, expected, sizeof(actual)) != 0)
		{
			printf("    " RED "[%d] %s Buffer mismatch (check NUL padding / bounds)\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
	}
	return (error);
}
