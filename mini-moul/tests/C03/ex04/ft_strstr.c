#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex04/ft_strstr.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*str;
	char	*to_find;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Match in the middle", .str = "42 1337 Network piscine Benguerir", .to_find = "piscine"},
		{.desc = "Match at the beginning", .str = "piscine Benguerir", .to_find = "piscine"},
		{.desc = "Match at the end", .str = "The quick brown fox jumps over the lazy dog", .to_find = "dog"},
		{.desc = "Empty to_find in non-empty str", .str = "Hello 42", .to_find = ""},
		{.desc = "Empty to_find in empty str", .str = "", .to_find = ""},
		{.desc = "Non-empty to_find in empty str", .str = "", .to_find = "a"},
		{.desc = "to_find longer than str", .str = "ab", .to_find = "abcd"},
		{.desc = "Not found", .str = "42 Network piscine", .to_find = "abcd"},
		{.desc = "Overlapping partial match (aaab / aab)", .str = "aaab", .to_find = "aab"},
		{.desc = "Needle characters are present only separately", .str = "-62,47 +62,47", .to_find = "-47"},
		{.desc = "Overlapping partial match (ababc / abc)", .str = "ababc", .to_find = "abc"},
		{.desc = "Needle characters are present only separately", .str = "-62,47 +62,47", .to_find = "-47"},
		{.desc = "Partial match at end of str", .str = "hello wor", .to_find = "world"},
		{.desc = "False partial characters (-62,47 +62,47 / -47)", .str = "-62,47 +62,47", .to_find = "-47"},
		{.desc = "Multiple occurrences returns first", .str = "foo bar foo baz", .to_find = "foo"},
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
		char	*expected = strstr(tests[i].str, tests[i].to_find);
		char	*result = ft_strstr(tests[i].str, tests[i].to_find);

		if (result != expected)
		{
			printf("    " RED "[%d] %s Expected %p (\"%s\"), got %p (\"%s\")\n" DEFAULT,
				i + 1, tests[i].desc, (void *)expected, expected ? expected : "NULL",
				(void *)result, result ? result : "NULL");
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
	}
	return (error);
}
