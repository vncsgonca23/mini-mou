#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex00/ft_strcpy.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*src;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Copy empty string", .src = ""},
		{.desc = "Copy single character", .src = "A"},
		{.desc = "Copy standard string", .src = "FGccjqWCcYr"},
		{.desc = "Copy string with spaces and symbols", .src = "Hello, 42! \t\n"},
		{.desc = "Copy long string", .src = "gVepWWXynLOXqCHCscGrowUZSebdZIfy"},
		{.desc = "Copy string with embedded null", .src = "TcXF\0ignored"},
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
		size_t	src_len = strlen(tests[i].src);

		memset(dest, 'X', sizeof(dest));
		dest[sizeof(dest) - 1] = '\0';

		char	*result = ft_strcpy(dest, tests[i].src);

		if (result != dest)
		{
			printf("    " RED "[%d] %s Did not return dest pointer\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else if (memcmp(dest, tests[i].src, src_len + 1) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].src, dest);
			error -= 1;
		}
		else if (dest[src_len + 1] != 'X')
		{
			printf("    " RED "[%d] %s Wrote past NUL terminator\n" DEFAULT, i + 1, tests[i].desc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
	}
	return (error);
}
