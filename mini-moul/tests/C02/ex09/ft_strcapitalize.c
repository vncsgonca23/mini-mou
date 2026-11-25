#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex09/ft_strcapitalize.c"
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
		{
			.desc = "Subject example (English)",
			.src = "hi, how are you? 42words forty-two; fifty+and+one",
			.expected = "Hi, How Are You? 42words Forty-Two; Fifty+And+One",
		},
		{
			.desc = "Subject example (French)",
			.src = "salut, comment tu vas ? 42mots quarante-deux; cinquante+et+un",
			.expected = "Salut, Comment Tu Vas ? 42mots Quarante-Deux; Cinquante+Et+Un",
		},
		{
			.desc = "Empty string",
			.src = "",
			.expected = "",
		},
		{
			.desc = "Single lowercase letter",
			.src = "a",
			.expected = "A",
		},
		{
			.desc = "All uppercase word",
			.src = "HELLO",
			.expected = "Hello",
		},
		{
			.desc = "Uppercase after digits stays lowercase",
			.src = "42WORDS 0A9Z",
			.expected = "42words 0a9z",
		},
		{
			.desc = "Leading and trailing spaces",
			.src = "   hello   world   ",
			.expected = "   Hello   World   ",
		},
		{
			.desc = "Alphanumeric alternating",
			.src = "a1B2c3D4e5F6g7H8i9J0",
			.expected = "A1b2c3d4e5f6g7h8i9j0",
		},
		{
			.desc = "Non-alphanumeric separators ([, _, `, {)",
			.src = "[aB_cD`eF{gH-9I+jK",
			.expected = "[Ab_Cd`Ef{Gh-9i+Jk",
		},
		{
			.desc = "Only symbols",
			.src = "!@#$%^&*()_+-=[]{}\\|;':\",./<>?",
			.expected = "!@#$%^&*()_+-=[]{}\\|;':\",./<>?",
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
		char	*buf = strdup(tests[i].src);
		char	*ret = ft_strcapitalize(buf);

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
