#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex06/ft_print_comb2.c"
#include "../../../utils/constants.h"

size_t	build_expected(char *out, size_t cap);
int		run_call(int num, const char *desc, const char *expected, size_t expected_len);

int	main(void)
{
	char	expected[40000];
	size_t	expected_len = build_expected(expected, sizeof(expected));
	int		error = 0;

	if (run_call(1, "ft_print_comb2()", expected, expected_len) != 0)
		error -= 1;
	if (run_call(2, "ft_print_comb2() second call", expected, expected_len) != 0)
		error -= 1;
	return (error);
}

size_t	build_expected(char *out, size_t cap)
{
	size_t	pos = 0;

	for (int a = 0; a <= 98; a++)
	{
		for (int b = a + 1; b <= 99; b++)
		{
			if (pos > 0)
				pos += (size_t)snprintf(out + pos, cap - pos, ", ");
			pos += (size_t)snprintf(out + pos, cap - pos, "%02d %02d", a, b);
		}
	}
	return (pos);
}

int	run_call(int num, const char *desc, const char *expected, size_t expected_len)
{
	char	buffer[40000];

	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));

	int	saved_stdout = dup(STDOUT_FILENO);
	int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	ft_print_comb2();

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
			num, desc, expected_len, len, diff);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s output matched (%zu bytes) as expected\n" DEFAULT,
		num, desc, len);
	return (0);
}
