#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "../../../../ex04/ft_is_negative.c"
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
		{.desc = "ft_is_negative(8)", .n = 8, .expected = "P"},
		{.desc = "ft_is_negative(1)", .n = 1, .expected = "P"},
		{.desc = "ft_is_negative(0)", .n = 0, .expected = "P"},
		{.desc = "ft_is_negative(-1)", .n = -1, .expected = "N"},
		{.desc = "ft_is_negative(-115)", .n = -115, .expected = "N"},
		{.desc = "ft_is_negative(INT_MIN)", .n = INT_MIN, .expected = "N"},
		{.desc = "ft_is_negative(INT_MAX)", .n = INT_MAX, .expected = "P"},
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

		ft_is_negative(tests[i].n);

		fflush(stdout);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);

		FILE	*fp = fopen("output.txt", "rb");
		size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
		fclose(fp);
		remove("output.txt");
		buffer[len] = '\0';

		if (len != 1 || memcmp(buffer, tests[i].expected, 1) != 0)
		{
			printf("    " RED "[%d] %s Expected \"%s\" (1 byte), got \"%s\" (%zu bytes)\n" DEFAULT,
				i + 1, tests[i].desc, tests[i].expected, buffer, len);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output \"%s\" as expected\n" DEFAULT,
				i + 1, tests[i].desc, buffer);
	}
	return (error);
}
