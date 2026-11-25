#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "../../../../ex04/ft_putnbr_base.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		nbr;
	char	*base;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "Zero in decimal", .nbr = 0, .base = "0123456789", .expected = "0"},
		{.desc = "Zero in custom base (poneyvif)", .nbr = 0, .base = "poneyvif", .expected = "p"},
		{.desc = "Positive decimal", .nbr = 894867, .base = "0123456789", .expected = "894867"},
		{.desc = "Negative binary (-12)", .nbr = -12, .base = "01", .expected = "-1100"},
		{.desc = "Custom octal base poneyvif (40)", .nbr = 40, .base = "poneyvif", .expected = "vp"},
		{.desc = "Hexadecimal uppercase (255)", .nbr = 255, .base = "0123456789ABCDEF", .expected = "FF"},
		{.desc = "Hexadecimal lowercase (-53)", .nbr = -53, .base = "0123456789abcdef", .expected = "-35"},
		{.desc = "INT_MAX in decimal", .nbr = INT_MAX, .base = "0123456789", .expected = "2147483647"},
		{.desc = "INT_MAX in poneyvif", .nbr = INT_MAX, .base = "poneyvif", .expected = "offffffffff"},
		{.desc = "INT_MIN in decimal", .nbr = INT_MIN, .base = "0123456789", .expected = "-2147483648"},
		{.desc = "INT_MIN in binary", .nbr = INT_MIN, .base = "01", .expected = "-10000000000000000000000000000000"},
		{.desc = "INT_MIN in hexadecimal", .nbr = INT_MIN, .base = "0123456789ABCDEF", .expected = "-80000000"},
		{.desc = "INT_MIN in poneyvif", .nbr = INT_MIN, .base = "poneyvif", .expected = "-npppppppppp"},
		{.desc = "Invalid: empty base", .nbr = 42, .base = "", .expected = ""},
		{.desc = "Invalid: single char base", .nbr = 42, .base = "0", .expected = ""},
		{.desc = "Invalid: duplicate adjacent chars", .nbr = 42, .base = "abccde", .expected = ""},
		{.desc = "Invalid: duplicate non-adjacent chars", .nbr = 42, .base = "0123456780", .expected = ""},
		{.desc = "Invalid: base contains '+'", .nbr = 42, .base = "012+456", .expected = ""},
		{.desc = "Invalid: base contains '-'", .nbr = 42, .base = "012-456", .expected = ""},
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
		fflush(stdout);

		char	buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		int	saved_stdout = dup(STDOUT_FILENO);
		int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);

		ft_putnbr_base(tests[i].nbr, tests[i].base);

		fflush(stdout);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);

		FILE	*fp = fopen("output.txt", "rb");
		size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
		fclose(fp);
		remove("output.txt");
		buffer[len] = '\0';

		size_t	expected_len = strlen(tests[i].expected);
		if (len != expected_len || memcmp(buffer, tests[i].expected, expected_len) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\" (%zu bytes), got \"%s\" (%zu bytes)\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, expected_len, buffer, len);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output \"%s\" as expected\n" DEFAULT,
				i + 1, tests[i].desc, buffer);
	}
	return (error);
}
