#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex03/ft_count_if.c"
#include "../../../utils/constants.h"

static int	is_nonempty(char *str)
{
	if (str[0] != '\0')
		return (-5);
	return (0);
}

static int	starts_with_a(char *str)
{
	return (str[0] == 'a') ? 3 : 0;
}

typedef struct s_test
{
	char	*desc;
	char	**tab;
	int		length;
	int		(*f)(char *);
	int		expected;
}	t_test;

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "length == 0 -> 0",
			.tab = (char *[]){"a", "b"},
			.length = 0,
			.f = &starts_with_a,
			.expected = 0,
		},
		{
			.desc = "No matches -> 0",
			.tab = (char *[]){"bb", "cc", "dd"},
			.length = 3,
			.f = &starts_with_a,
			.expected = 0,
		},
		{
			.desc = "Some matches (f returns 3 -> counts +1 each)",
			.tab = (char *[]){"apple", "banana", "apricot", "cherry", "avocado"},
			.length = 5,
			.f = &starts_with_a,
			.expected = 3,
		},
		{
			.desc = "All match (f returns -5 -> counts +1 each)",
			.tab = (char *[]){"x", "y", "z"},
			.length = 3,
			.f = &is_nonempty,
			.expected = 3,
		},
		{
			.desc = "Partial length stops early",
			.tab = (char *[]){"a1", "b2", "a3", "a4"},
			.length = 2,
			.f = &starts_with_a,
			.expected = 1,
		},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);
	int		error = 0;

	for (int i = 0; i < count; i++)
	{
		int	res = ft_count_if(tests[i].tab, tests[i].length, tests[i].f);
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
