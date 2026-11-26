#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex08/ft_print_combn.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	int		n;
}	t_test;

void	gen_combn(int n, int depth, int start, char *cur, char *out, size_t *pos);
int		run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_print_combn(1)", .n = 1},
		{.desc = "ft_print_combn(2)", .n = 2},
		{.desc = "ft_print_combn(3)", .n = 3},
		{.desc = "ft_print_combn(4)", .n = 4},
		{.desc = "ft_print_combn(5)", .n = 5},
		{.desc = "ft_print_combn(6)", .n = 6},
		{.desc = "ft_print_combn(7)", .n = 7},
		{.desc = "ft_print_combn(8)", .n = 8},
		{.desc = "ft_print_combn(9)", .n = 9},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

void	gen_combn(int n, int depth, int start, char *cur, char *out, size_t *pos)
{
	if (depth == n)
	{
		if (*pos > 0)
		{
			out[(*pos)++] = ',';
			out[(*pos)++] = ' ';
		}
		memcpy(out + *pos, cur, (size_t)n);
		*pos += (size_t)n;
		return ;
	}
	for (int d = start; d <= 10 - (n - depth); d++)
	{
		cur[depth] = (char)('0' + d);
		gen_combn(n, depth + 1, d + 1, cur, out, pos);
	}
}

int	run_tests(t_test *tests, int count)
{
	int	i;
	int	error = 0;

	for (i = 0; i < count; i++)
	{
		char	expected[4096];
		char	cur[16];
		char	buffer[4096];
		size_t	expected_len = 0;

		memset(expected, 0, sizeof(expected));
		memset(buffer, 0, sizeof(buffer));
		gen_combn(tests[i].n, 0, 0, cur, expected, &expected_len);
		expected[expected_len] = '\0';

		fflush(stdout);
		int	saved_stdout = dup(STDOUT_FILENO);
		int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);

		ft_print_combn(tests[i].n);

		fflush(stdout);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);

		FILE	*fp = fopen("output.txt", "rb");
		size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
		fclose(fp);
		remove("output.txt");
		buffer[len] = '\0';

		if (len != expected_len || memcmp(buffer, expected, expected_len) != 0)
		{
			size_t	diff = 0;
			while (diff < len && diff < expected_len && buffer[diff] == expected[diff])
				diff++;
			printf("    " RED "[%d] %s Expected %zu bytes, got %zu bytes (first diff at byte %zu)\n" DEFAULT,
				i + 1, tests[i].desc, expected_len, len, diff);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output matched (%zu bytes) as expected\n" DEFAULT,
				i + 1, tests[i].desc, len);
	}
	return (error);
}
