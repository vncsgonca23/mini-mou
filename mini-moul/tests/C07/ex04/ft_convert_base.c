#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../utils/constants.h"

char	*ft_convert_base(char *nbr, char *base_from, char *base_to);

typedef struct s_test
{
	char	*desc;
	char	*nbr;
	char	*base_from;
	char	*base_to;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Positive decimal to binary", .nbr = "42", .base_from = "0123456789", .base_to = "01", .expected = "101010"},
		{.desc = "Negative decimal to binary", .nbr = "-42", .base_from = "0123456789", .base_to = "01", .expected = "-101010"},
		{.desc = "Binary to hexadecimal", .nbr = "101010", .base_from = "01", .base_to = "0123456789ABCDEF", .expected = "2A"},
		{.desc = "Zero to custom base (poneyvif)", .nbr = "0", .base_from = "0123456789", .base_to = "poneyvif", .expected = "p"},
		{.desc = "Negative zero must NOT produce '-' prefix", .nbr = "  ---0", .base_from = "0123456789", .base_to = "01", .expected = "0"},
		{.desc = "Whitespace, multiple signs and trailing junk", .nbr = " \t\n\v\f\r---+--+2Azz", .base_from = "0123456789ABCDEF", .base_to = "0123456789", .expected = "-42"},
		{.desc = "Even minus signs -> positive (no '+' prefix)", .nbr = "  ++--42", .base_from = "0123456789", .base_to = "0123456789", .expected = "42"},
		{.desc = "INT_MAX decimal to hex", .nbr = "2147483647", .base_from = "0123456789", .base_to = "0123456789ABCDEF", .expected = "7FFFFFFF"},
		{.desc = "INT_MIN decimal to hex", .nbr = "-2147483648", .base_from = "0123456789", .base_to = "0123456789ABCDEF", .expected = "-80000000"},
		{.desc = "INT_MIN binary to decimal", .nbr = "-10000000000000000000000000000000", .base_from = "01", .base_to = "0123456789", .expected = "-2147483648"},
		{.desc = "INT_MIN decimal to poneyvif", .nbr = "-2147483648", .base_from = "0123456789", .base_to = "poneyvif", .expected = "-npppppppppp"},
		{.desc = "Non-base input returns zero in base_to", .nbr = "xyz", .base_from = "0123456789", .base_to = "poneyvif", .expected = "p"},
		{.desc = "Invalid base_from: duplicate char", .nbr = "42", .base_from = "01234567899", .base_to = "01", .expected = NULL},
		{.desc = "Invalid base_from: length 1", .nbr = "0", .base_from = "0", .base_to = "01", .expected = NULL},
		{.desc = "Invalid base_from: empty", .nbr = "42", .base_from = "", .base_to = "01", .expected = NULL},
		{.desc = "Invalid base_from: contains '+'", .nbr = "42", .base_from = "012+4", .base_to = "01", .expected = NULL},
		{.desc = "Invalid base_from: contains whitespace", .nbr = "42", .base_from = "012 4", .base_to = "01", .expected = NULL},
		{.desc = "Invalid base_to: length 1", .nbr = "42", .base_from = "0123456789", .base_to = "5", .expected = NULL},
		{.desc = "Invalid base_to: duplicate char", .nbr = "42", .base_from = "0123456789", .base_to = "010", .expected = NULL},
		{.desc = "Invalid base_to: contains '-'", .nbr = "42", .base_from = "0123456789", .base_to = "01-2", .expected = NULL},
		{.desc = "Invalid base_to: contains tab", .nbr = "42", .base_from = "0123456789", .base_to = "01\t2", .expected = NULL},
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
		char	*result = ft_convert_base(tests[i].nbr, tests[i].base_from, tests[i].base_to);

		if (!result && !tests[i].expected)
			printf("  " GREEN CHECKMARK GREY " [%d] %s Returned NULL as expected\n" DEFAULT, i + 1, tests[i].desc);
		else if (!result || !tests[i].expected)
		{
			printf("    " RED "[%d] %s Expected %s, got %s\n" DEFAULT,
				i + 1, tests[i].desc,
				tests[i].expected ? tests[i].expected : "NULL",
				result ? result : "NULL");
			error -= 1;
		}
		else if (strcmp(result, tests[i].expected) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\", got \"%s\"\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, result);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s got \"%s\" as expected\n" DEFAULT,
				i + 1, tests[i].desc, result);
		free(result);
	}
	return (error);
}
