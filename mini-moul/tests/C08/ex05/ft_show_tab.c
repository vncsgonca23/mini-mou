#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "resources/ft_stock_str.h"
#include "../../../utils/constants.h"

void	ft_show_tab(struct s_stock_str *par);

static int	run_show_test(int num, const char *desc, t_stock_str *par, const char *expected)
{
	char	buffer[2048];
	size_t	exp_len = strlen(expected);

	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));

	int	saved_stdout = dup(STDOUT_FILENO);
	int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	ft_show_tab(par);

	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	FILE	*fp = fopen("output.txt", "rb");
	size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
	fclose(fp);
	remove("output.txt");
	buffer[len] = '\0';

	if (len != exp_len || memcmp(buffer, expected, exp_len) != 0)
	{
		printf("    " RED "[%d] %s Expected %zu bytes, got %zu bytes\n" DEFAULT,
			num, desc, exp_len, len);
		printf("    " RED "Expected:\n%s" DEFAULT, expected);
		printf("    " RED "Got:\n%s" DEFAULT, buffer);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s Passed (%zu bytes)\n" DEFAULT, num, desc, len);
	return (0);
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Normal strings, empty string (size 0), and modified copy != str */
	char		copy1[] = "HELLO_MODIFIED";
	char		copy2[] = "";
	char		copy3[] = "42sp";
	t_stock_str	tab1[] = {
		{.size = 5, .str = "hello", .copy = copy1},
		{.size = 0, .str = "", .copy = copy2},
		{.size = 12, .str = "0123456789ab", .copy = copy3},
		{.size = 0, .str = 0, .copy = 0},
	};
	const char	*exp1 =
		"hello\n5\nHELLO_MODIFIED\n"
		"\n0\n\n"
		"0123456789ab\n12\n42sp\n";

	if (run_show_test(1, "Strings, size=0, multi-digit size & modified copy", tab1, exp1) != 0)
		error -= 1;

	/* Test 2: Empty tab (only sentinel) -> 0 bytes */
	t_stock_str	tab2[] = {
		{.size = 0, .str = 0, .copy = 0},
	};
	if (run_show_test(2, "Empty tab (sentinel only -> 0 bytes)", tab2, "") != 0)
		error -= 1;

	return (error);
}
