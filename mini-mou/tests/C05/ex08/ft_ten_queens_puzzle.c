#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex08/ft_ten_queens_puzzle.c"
#include "../../../utils/constants.h"

static void	test_gen_queens(int col, int *board, char *out, size_t *pos, int *count)
{
	if (col == 10)
	{
		for (int i = 0; i < 10; i++)
			out[(*pos)++] = (char)('0' + board[i]);
		out[(*pos)++] = '\n';
		(*count)++;
		return ;
	}
	for (int row = 0; row < 10; row++)
	{
		int	safe = 1;
		for (int prev = 0; prev < col; prev++)
		{
			int	diff = board[prev] - row;
			if (diff == 0 || diff == col - prev || diff == prev - col)
			{
				safe = 0;
				break ;
			}
		}
		if (safe)
		{
			board[col] = row;
			test_gen_queens(col + 1, board, out, pos, count);
		}
	}
}

static int	run_call(int num, const char *desc, const char *expected, size_t expected_len, int expected_count)
{
	char	buffer[16384];
	int		ret;

	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));

	int	saved_stdout = dup(STDOUT_FILENO);
	int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	ret = ft_ten_queens_puzzle();

	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	FILE	*fp = fopen("output.txt", "rb");
	size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
	fclose(fp);
	remove("output.txt");
	buffer[len] = '\0';

	if (ret != expected_count)
	{
		printf("    " RED "[%d] %s Expected return %d, got %d\n" DEFAULT,
			num, desc, expected_count, ret);
		return (-1);
	}
	if (len != expected_len || memcmp(buffer, expected, expected_len) != 0)
	{
		size_t	diff = 0;
		while (diff < len && diff < expected_len && buffer[diff] == expected[diff])
			diff++;
		printf("    " RED "[%d] %s Output mismatch: expected %zu bytes, got %zu bytes (first diff at byte %zu)\n" DEFAULT,
			num, desc, expected_len, len, diff);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s Returned %d and matched all %zu bytes\n" DEFAULT,
		num, desc, ret, len);
	return (0);
}

int	main(void)
{
	char	expected[16384];
	int		board[10];
	size_t	expected_len = 0;
	int		expected_count = 0;
	int		error = 0;

	test_gen_queens(0, board, expected, &expected_len, &expected_count);
	expected[expected_len] = '\0';

	if (run_call(1, "ft_ten_queens_puzzle()", expected, expected_len, expected_count) != 0)
		error -= 1;
	if (run_call(2, "ft_ten_queens_puzzle() second call", expected, expected_len, expected_count) != 0)
		error -= 1;
	return (error);
}
