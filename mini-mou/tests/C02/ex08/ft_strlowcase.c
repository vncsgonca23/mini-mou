#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex08/ft_strlowcase.c"
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
		{.desc = "Full uppercase alphabet", .src = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", .expected = "abcdefghijklmnopqrstuvwxyz"},
		{.desc = "Already lowercase", .src = "hello world", .expected = "hello world"},
		{.desc = "Mixed case", .src = "agsKofnkFgbsdv", .expected = "agskofnkfgbsdv"},
		{.desc = "Boundaries around A-Z (@A Z[)", .src = "@A Z[`a z{", .expected = "@a z[`a z{"},
		{.desc = "Digits and symbols", .src = "23_-3FREWG4WVFSD4!", .expected = "23_-3frewg4wvfsd4!"},
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
		char	*buf = strdup(tests[i].src);
		char	*ret = ft_strlowcase(buf);

		if (ret != buf)
		{
			printf("    " RED "[%d] %s Did not return original str pointer\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else if (strcmp(buf, tests[i].expected) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, buf);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
		free(buf);
	}
	return (error);
}
