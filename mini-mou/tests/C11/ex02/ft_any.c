#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex02/ft_any.c"
#include "../../../utils/constants.h"

static int	has_digit(char *str)
{
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			return (42);
		str++;
	}
	return (0);
}

static int	returns_negative_on_match(char *str)
{
	if (strcmp(str, "hit") == 0)
		return (-7);
	return (0);
}

typedef struct s_test
{
	char	*desc;
	char	**tab;
	int		(*f)(char *);
	int		expected;
}	t_test;

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "Empty NULL-terminated array -> 0",
			.tab = (char *[]){NULL},
			.f = &has_digit,
			.expected = 0,
		},
		{
			.desc = "No matching element -> 0",
			.tab = (char *[]){"hello", "world", "", NULL},
			.f = &has_digit,
			.expected = 0,
		},
		{
			.desc = "First element matches (f returns 42 -> must return 1)",
			.tab = (char *[]){"42", "abc", "def", NULL},
			.f = &has_digit,
			.expected = 1,
		},
		{
			.desc = "Last element matches -> 1",
			.tab = (char *[]){"abc", "def", "x9", NULL},
			.f = &has_digit,
			.expected = 1,
		},
		{
			.desc = "f returns negative non-zero (-7) -> must return 1",
			.tab = (char *[]){"miss", "hit", "miss", NULL},
			.f = &returns_negative_on_match,
			.expected = 1,
		},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);
	int		error = 0;

	for (int i = 0; i < count; i++)
	{
		int	res = ft_any(tests[i].tab, tests[i].f);
		if (res != tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, res);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, res);
	}
	return (error);
}
