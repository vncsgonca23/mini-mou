#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex01/ft_print_alphabet.c"
#include "../../../utils/constants.h"

int	run_call(int num, const char *desc);

int	main(void)
{
	int	error = 0;

	if (run_call(1, "ft_print_alphabet()") != 0)
		error -= 1;
	if (run_call(2, "ft_print_alphabet() second call") != 0)
		error -= 1;
	return (error);
}

int	run_call(int num, const char *desc)
{
	const char	*expected_output = "abcdefghijklmnopqrstuvwxyz";
	size_t		expected_len = strlen(expected_output);
	char		buffer[1024];

	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));

	int	saved_stdout = dup(STDOUT_FILENO);
	int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	ft_print_alphabet();

	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	FILE	*fp = fopen("output.txt", "rb");
	size_t	len = fread(buffer, 1, sizeof(buffer) - 1, fp);
	fclose(fp);
	remove("output.txt");
	buffer[len] = '\0';

	if (len != expected_len || memcmp(buffer, expected_output, expected_len) != 0)
	{
		printf("    " RED "[%d] %s Expected \"%s\" (%zu bytes), got \"%s\" (%zu bytes)\n" DEFAULT,
			num, desc, expected_output, expected_len, buffer, len);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s output \"%s\" as expected\n" DEFAULT,
		num, desc, buffer);
	return (0);
}
