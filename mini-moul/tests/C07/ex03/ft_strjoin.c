#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex03/ft_strjoin.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		size;
	char	**strs;
	char	*sep;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "size == 0 with strs = NULL (must return freeable \"\")",
			.size = 0,
			.strs = NULL,
			.sep = ",",
			.expected = "",
		},
		{
			.desc = "size == 1 (no separator added)",
			.size = 1,
			.strs = (char *[]){"Hello"},
			.sep = ", ",
			.expected = "Hello",
		},
		{
			.desc = "size == 3 with \", \" separator",
			.size = 3,
			.strs = (char *[]){"Hello", "world", "!"},
			.sep = ", ",
			.expected = "Hello, world, !",
		},
		{
			.desc = "size == 4 with empty strings mixed in",
			.size = 4,
			.strs = (char *[]){"", "Hello", "", "world"},
			.sep = "-",
			.expected = "-Hello--world",
		},
		{
			.desc = "All empty strings with separator",
			.size = 3,
			.strs = (char *[]){"", "", ""},
			.sep = "::",
			.expected = "::::",
		},
		{
			.desc = "Empty separator",
			.size = 3,
			.strs = (char *[]){"42", " ", "School"},
			.sep = "",
			.expected = "42 School",
		},
		{
			.desc = "Multi-character separator",
			.size = 2,
			.strs = (char *[]){"left", "right"},
			.sep = " <-> ",
			.expected = "left <-> right",
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
		char	*result = ft_strjoin(tests[i].size, tests[i].strs, tests[i].sep);

		if (result == NULL)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got NULL\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected);
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
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (\"%s\")\n" DEFAULT,
				i + 1, tests[i].desc, result);
		free(result);
	}
	return (error);
}
