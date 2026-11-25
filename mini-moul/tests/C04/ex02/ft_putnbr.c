#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "../../../../ex02/ft_putnbr.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		n;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_putnbr(0)", .n = 0, .expected = "0"},
		{.desc = "ft_putnbr(1)", .n = 1, .expected = "1"},
		{.desc = "ft_putnbr(-1)", .n = -1, .expected = "-1"},
		{.desc = "ft_putnbr(9)", .n = 9, .expected = "9"},
		{.desc = "ft_putnbr(-9)", .n = -9, .expected = "-9"},
		{.desc = "ft_putnbr(10)", .n = 10, .expected = "10"},
		{.desc = "ft_putnbr(-10)", .n = -10, .expected = "-10"},
		{.desc = "ft_putnbr(42)", .n = 42, .expected = "42"},
		{.desc = "ft_putnbr(-42)", .n = -42, .expected = "-42"},
		{.desc = "ft_putnbr(100)", .n = 100, .expected = "100"},
		{.desc = "ft_putnbr(-100)", .n = -100, .expected = "-100"},
		{.desc = "ft_putnbr(10000)", .n = 10000, .expected = "10000"},
		{.desc = "ft_putnbr(-10000)", .n = -10000, .expected = "-10000"},
		{.desc = "ft_putnbr(2147483646)", .n = 2147483646, .expected = "2147483646"},
		{.desc = "ft_putnbr(-2147483647)", .n = -2147483647, .expected = "-2147483647"},
		{.desc = "ft_putnbr(INT_MAX)", .n = INT_MAX, .expected = "2147483647"},
		{.desc = "ft_putnbr(INT_MIN)", .n = INT_MIN, .expected = "-2147483648"},
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

		ft_putnbr(tests[i].n);

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
