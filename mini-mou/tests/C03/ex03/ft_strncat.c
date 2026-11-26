#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex03/ft_strncat.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char			*desc;
	char			*dest_init;
	char			*src;
	unsigned int	nb;
	char			*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "nb = 0 (must not append)", .dest_init = "Hello", .src = "World", .nb = 0, .expected = "Hello"},
		{.desc = "nb < strlen(src)", .dest_init = "Hello", .src = "World", .nb = 2, .expected = "HelloWo"},
		{.desc = "nb == strlen(src)", .dest_init = "Hello ", .src = "World", .nb = 5, .expected = "Hello World"},
		{.desc = "nb > strlen(src)", .dest_init = "Hello", .src = "!", .nb = 10, .expected = "Hello!"},
		{.desc = "Append empty to non-empty", .dest_init = "Hello", .src = "", .nb = 5, .expected = "Hello"},
		{.desc = "Append to empty string", .dest_init = "", .src = "42School", .nb = 2, .expected = "42"},
		{.desc = "Append empty to empty", .dest_init = "", .src = "", .nb = 5, .expected = ""},
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
		char	dest[64];
		size_t	exp_len = strlen(tests[i].expected);

		memset(dest, 'X', sizeof(dest));
		strcpy(dest, tests[i].dest_init);

		char	*ret = ft_strncat(dest, tests[i].src, tests[i].nb);

		if (ret != dest)
		{
			printf("    " RED "[%d] %s Did not return dest pointer\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else if (memcmp(dest, tests[i].expected, exp_len + 1) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, dest);
			error -= 1;
		}
		else if (dest[exp_len + 1] != 'X')
		{
			printf("    " RED "[%d] %s Wrote past NUL terminator\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (\"%s\")\n" DEFAULT, i + 1, tests[i].desc, dest);
	}
	return (error);
}
