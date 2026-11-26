#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex10/ft_strlcpy.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char			*desc;
	char			*src;
	unsigned int	size;
	unsigned int	expected_len;
	char			*expected;
	int				expect_untouched;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "size > strlen(src)",
			.src = "World!",
			.size = 10,
			.expected_len = 6,
			.expected = "World!",
			.expect_untouched = 0,
		},
		{
			.desc = "size == strlen(src) + 1 (exact fit)",
			.src = "Hello",
			.size = 6,
			.expected_len = 5,
			.expected = "Hello",
			.expect_untouched = 0,
		},
		{
			.desc = "size == strlen(src) (truncate 1 char)",
			.src = "Hello",
			.size = 5,
			.expected_len = 5,
			.expected = "Hell",
			.expect_untouched = 0,
		},
		{
			.desc = "size < strlen(src) (truncate)",
			.src = "Hello, World!",
			.size = 10,
			.expected_len = 13,
			.expected = "Hello, Wo",
			.expect_untouched = 0,
		},
		{
			.desc = "size == 1 (only NUL terminator)",
			.src = "Hello, World!",
			.size = 1,
			.expected_len = 13,
			.expected = "",
			.expect_untouched = 0,
		},
		{
			.desc = "size == 0 (must NOT modify dest)",
			.src = "Hello, World!",
			.size = 0,
			.expected_len = 13,
			.expected = "",
			.expect_untouched = 1,
		},
		{
			.desc = "Empty src with size > 0",
			.src = "",
			.size = 5,
			.expected_len = 0,
			.expected = "",
			.expect_untouched = 0,
		},
		{
			.desc = "Empty src with size == 0",
			.src = "",
			.size = 0,
			.expected_len = 0,
			.expected = "",
			.expect_untouched = 1,
		},
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
		char			dest[40];
		unsigned int	result_len;

		memset(dest, 'X', sizeof(dest));
		dest[sizeof(dest) - 1] = '\0';

		result_len = ft_strlcpy(dest, tests[i].src, tests[i].size);

		if (result_len != tests[i].expected_len)
		{
			printf("    " RED "[%d] %s Expected return %u, got %u\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_len, result_len);
			error -= 1;
		}
		else if (tests[i].expect_untouched)
		{
			if (dest[0] != 'X')
			{
				printf("    " RED "[%d] %s Modified dest when size == 0\n" DEFAULT,
					i + 1, tests[i].desc);
				error -= 1;
			}
			else
				printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (len %u)\n" DEFAULT,
					i + 1, tests[i].desc, result_len);
		}
		else if (strcmp(dest, tests[i].expected) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, dest);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (\"%s\", len %u)\n" DEFAULT,
				i + 1, tests[i].desc, dest, result_len);
	}
	return (error);
}
