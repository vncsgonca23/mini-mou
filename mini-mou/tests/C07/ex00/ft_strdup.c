#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex00/ft_strdup.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*src;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Empty string", .src = "", .expected = ""},
		{.desc = "Single character", .src = "4", .expected = "4"},
		{.desc = "Standard string", .src = "test", .expected = "test"},
		{.desc = "String with spaces and newlines", .src = "Hello\tWorld\n42!", .expected = "Hello\tWorld\n42!"},
		{.desc = "Long string", .src = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()", .expected = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()"},
		{.desc = "String with embedded NUL", .src = "test\0ignored", .expected = "test"},
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
		char	*result = ft_strdup(tests[i].src);

		if (result == NULL)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got NULL\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected);
			error -= 1;
			continue ;
		}
		if (result == tests[i].src)
		{
			printf("    " RED "[%d] %s Returned original pointer instead of newly allocated copy\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
			continue ;
		}
		if (strcmp(result, tests[i].expected) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, result);
			error -= 1;
		}
		else
		{
			/* Verify allocated memory is writable */
			if (result[0] != '\0')
				result[0] = (result[0] == 'X') ? 'Y' : 'X';
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
		}
		free(result);
	}
	return (error);
}
