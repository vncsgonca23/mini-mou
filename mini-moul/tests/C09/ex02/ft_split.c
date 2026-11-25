#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex02/ft_split.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*str;
	char	*charset;
	char	**expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "Empty string with empty charset",
			.str = "",
			.charset = "",
			.expected = (char *[]){NULL},
		},
		{
			.desc = "Empty string with non-empty charset",
			.str = "",
			.charset = ",; ",
			.expected = (char *[]){NULL},
		},
		{
			.desc = "Only separators in string",
			.str = ",,;; ,;",
			.charset = ",; ",
			.expected = (char *[]){NULL},
		},
		{
			.desc = "Non-empty string with empty charset",
			.str = "hello world",
			.charset = "",
			.expected = (char *[]){"hello world", NULL},
		},
		{
			.desc = "No separators found in string",
			.str = "hello world",
			.charset = ",;",
			.expected = (char *[]){"hello world", NULL},
		},
		{
			.desc = "Leading, trailing and consecutive separators",
			.str = ",,hello,,,,world,,",
			.charset = ",",
			.expected = (char *[]){"hello", "world", NULL},
		},
		{
			.desc = "Multiple separator characters in charset",
			.str = "a,b;c:d e\tf",
			.charset = ",;: \t",
			.expected = (char *[]){"a", "b", "c", "d", "e", "f", NULL},
		},
		{
			.desc = "Repeated multi-char separators",
			.str = "aaabbbaaaccc",
			.charset = "ab",
			.expected = (char *[]){"ccc", NULL},
		},
		{
			.desc = "Single character word",
			.str = "4",
			.charset = ",",
			.expected = (char *[]){"4", NULL},
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
		char	**result = ft_split(tests[i].str, tests[i].charset);

		if (result == NULL)
		{
			printf("    " RED "[%d] %s Returned NULL instead of NULL-terminated array\n" DEFAULT,
				i + 1, tests[i].desc);
			error -= 1;
			continue ;
		}
		int	j = 0;
		int	ok = 1;
		while (tests[i].expected[j] && result[j])
		{
			if (strcmp(tests[i].expected[j], result[j]) != 0)
			{
				printf("    " RED "[%d] %s Element %d: expected \"%s\", got \"%s\"\n" DEFAULT,
					i + 1, tests[i].desc, j, tests[i].expected[j], result[j]);
				ok = 0;
				error -= 1;
				break ;
			}
			j++;
		}
		if (ok && (tests[i].expected[j] != NULL || result[j] != NULL))
		{
			printf("    " RED "[%d] %s Element count mismatch at index %d\n" DEFAULT,
				i + 1, tests[i].desc, j);
			ok = 0;
			error -= 1;
		}
		if (ok)
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (%d word(s))\n" DEFAULT,
				i + 1, tests[i].desc, j);
		for (int k = 0; result[k]; k++)
			free(result[k]);
		free(result);
	}
	return (error);
}
