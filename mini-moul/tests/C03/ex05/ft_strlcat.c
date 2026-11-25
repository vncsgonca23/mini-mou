#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex05/ft_strlcat.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char			*desc;
	char			*dest_init;
	char			*src;
	unsigned int	size;
	char			*expected_dest;
	unsigned int	expected_ret;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "Full concatenation (size >= dest + src + 1)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 20,
			.expected_dest = "1337 42Born to code",
			.expected_ret = 19,
		},
		{
			.desc = "Truncation (dest_len < size < dest + src + 1)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 10,
			.expected_dest = "1337 42Bo",
			.expected_ret = 19,
		},
		{
			.desc = "size == dest_len + 1 (no space to append, only NUL)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 8,
			.expected_dest = "1337 42",
			.expected_ret = 19,
		},
		{
			.desc = "size == dest_len (returns size + src_len)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 7,
			.expected_dest = "1337 42",
			.expected_ret = 19,
		},
		{
			.desc = "size < dest_len (returns size + src_len, dest untouched)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 3,
			.expected_dest = "1337 42",
			.expected_ret = 15,
		},
		{
			.desc = "size == 0 (returns src_len, dest untouched)",
			.dest_init = "1337 42",
			.src = "Born to code",
			.size = 0,
			.expected_dest = "1337 42",
			.expected_ret = 12,
		},
		{
			.desc = "Empty dest with truncation",
			.dest_init = "",
			.src = "hello world",
			.size = 6,
			.expected_dest = "hello",
			.expected_ret = 11,
		},
		{
			.desc = "Empty dest with size == 1",
			.dest_init = "",
			.src = "hello",
			.size = 1,
			.expected_dest = "",
			.expected_ret = 5,
		},
		{
			.desc = "Empty src with non-empty dest",
			.dest_init = "hello",
			.src = "",
			.size = 10,
			.expected_dest = "hello",
			.expected_ret = 5,
		},
		{
			.desc = "Empty dest and empty src",
			.dest_init = "",
			.src = "",
			.size = 10,
			.expected_dest = "",
			.expected_ret = 0,
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
		char			dest[64];
		unsigned int	ret;

		memset(dest, 'X', sizeof(dest));
		strcpy(dest, tests[i].dest_init);

		ret = ft_strlcat(dest, tests[i].src, tests[i].size);

		if (ret != tests[i].expected_ret || strcmp(dest, tests[i].expected_dest) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\" (ret %u), got \"%s\" (ret %u)\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected_dest, tests[i].expected_ret, dest, ret);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (\"%s\", ret %u)\n" DEFAULT,
				i + 1, tests[i].desc, dest, ret);
	}
	return (error);
}
