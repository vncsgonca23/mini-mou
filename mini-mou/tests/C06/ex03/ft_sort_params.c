#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*argv[10];
	char	*expected;
}	t_test;

static int	run_prog(const char *bin, char *const argv[], char *out, size_t cap, size_t *out_len)
{
	int		pfd[2];
	pid_t	pid;
	int		status;

	if (pipe(pfd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		execv(bin, argv);
		_exit(127);
	}
	close(pfd[1]);
	*out_len = 0;
	while (*out_len < cap - 1)
	{
		ssize_t	r = read(pfd[0], out + *out_len, cap - 1 - *out_len);
		if (r <= 0)
			break ;
		*out_len += (size_t)r;
	}
	out[*out_len] = '\0';
	close(pfd[0]);
	waitpid(pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : -1);
}

int	main(void)
{
	const char	*bin = "./c06_ex03_bin";

	if (system("cc -Wall -Wextra -Werror ../ex03/ft_sort_params.c -o ./c06_ex03_bin") != 0)
	{
		printf("    " RED "[0] Failed to compile ../ex03/ft_sort_params.c\n" DEFAULT);
		return (-1);
	}

	t_test	tests[] = {
		{
			.desc = "No arguments (must output nothing)",
			.argv = {"./a.out", NULL},
			.expected = "",
		},
		{
			.desc = "Single argument",
			.argv = {"./a.out", "solo", NULL},
			.expected = "solo\n",
		},
		{
			.desc = "Basic ASCII sort (123 < hello < world)",
			.argv = {"./a.out", "hello", "world", "123", NULL},
			.expected = "123\nhello\nworld\n",
		},
		{
			.desc = "ASCII order: empty < digits < uppercase < lowercase",
			.argv = {"./a.out", "a", "Z", "2", "10", "A", "z", "", NULL},
			.expected = "\n10\n2\nA\nZ\na\nz\n",
		},
		{
			.desc = "Duplicates and common prefixes (ab < abc < abd)",
			.argv = {"./a.out", "abd", "ab", "abc", "ab", NULL},
			.expected = "ab\nab\nabc\nabd\n",
		},
		{
			.desc = "Already sorted & reverse sorted with argv[0] excluded",
			.argv = {"zzz_prog", "c", "b", "a", NULL},
			.expected = "a\nb\nc\n",
		},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);
	int		error = 0;

	for (int i = 0; i < count; i++)
	{
		char	buf[1024];
		size_t	len = 0;
		size_t	exp_len = strlen(tests[i].expected);
		int		rc = run_prog(bin, tests[i].argv, buf, sizeof(buf), &len);

		if (rc != 0 || len != exp_len || memcmp(buf, tests[i].expected, exp_len) != 0)
		{
			printf("    " RED "[%d] %s Expected %zu bytes, got %zu bytes (rc=%d)\n" DEFAULT,
				i + 1, tests[i].desc, exp_len, len, rc);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s Passed\n" DEFAULT, i + 1, tests[i].desc);
	}
	remove(bin);
	return (error);
}
