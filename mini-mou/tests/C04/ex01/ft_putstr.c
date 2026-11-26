#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex01/ft_putstr.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*str;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{
			.desc = "Output a simple string",
			.str = "hello world",
			.expected = "hello world",
		},
		{
			.desc = "Output an empty string",
			.str = "",
			.expected = "",
		},
		{
			.desc = "Output a single character",
			.str = "4",
			.expected = "4",
		},
		{
			.desc = "Output a string containing numbers",
			.str = "1234567890",
			.expected = "1234567890",
		},
		{
			.desc = "Output a string with newlines and tabs",
			.str = "line1\n\tline2\nline3",
			.expected = "line1\n\tline2\nline3",
		},
		{
			.desc = "Output a string containing special characters",
			.str = "!@#$%^&*()_+}{:\"?><,./';][=-`~",
			.expected = "!@#$%^&*()_+}{:\"?><,./';][=-`~",
		},
		{
			.desc = "Output a string with embedded null terminator",
			.str = "Hello\0World",
			.expected = "Hello",
		},
		{
			.desc = "Output a long string",
			.str = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.",
			.expected = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.",
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
		fflush(stdout);

		char	buffer[1024];
		memset(buffer, 0, sizeof(buffer));

		int	saved_stdout = dup(STDOUT_FILENO);
		int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);

		ft_putstr(tests[i].str);

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
			printf("  " GREEN CHECKMARK GREY " [%d] %s matched (%zu bytes) as expected\n" DEFAULT,
				i + 1, tests[i].desc, len);
	}
	return (error);
}
