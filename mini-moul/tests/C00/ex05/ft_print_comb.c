#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex05/ft_print_comb.c"
#include "../../../utils/constants.h"

int	run_call(int num, const char *desc);

int	main(void)
{
	int	error = 0;

	if (run_call(1, "ft_print_comb()") != 0)
		error -= 1;
	if (run_call(2, "ft_print_comb() second call") != 0)
		error -= 1;
	return (error);
}

int	run_call(int num, const char *desc)
{
	const char	*expected_output = "012, 013, 014, 015, 016, 017, 018, 019, 023, 024, 025, 026, 027, 028, 029, 034, 035, 036, 037, 038, 039, 045, 046, 047, 048, 049, 056, 057, 058, 059, 067, 068, 069, 078, 079, 089, 123, 124, 125, 126, 127, 128, 129, 134, 135, 136, 137, 138, 139, 145, 146, 147, 148, 149, 156, 157, 158, 159, 167, 168, 169, 178, 179, 189, 234, 235, 236, 237, 238, 239, 245, 246, 247, 248, 249, 256, 257, 258, 259, 267, 268, 269, 278, 279, 289, 345, 346, 347, 348, 349, 356, 357, 358, 359, 367, 368, 369, 378, 379, 389, 456, 457, 458, 459, 467, 468, 469, 478, 479, 489, 567, 568, 569, 578, 579, 589, 678, 679, 689, 789";
	size_t		expected_len = strlen(expected_output);
	char		buffer[2048];

	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));

	int	saved_stdout = dup(STDOUT_FILENO);
	int	output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);

	ft_print_comb();

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
		printf("    " RED "[%d] %s Expected %zu bytes, got %zu bytes\n" DEFAULT,
			num, desc, expected_len, len);
		printf("    " RED "Expected: \"%s\"\n" DEFAULT, expected_output);
		printf("    " RED "Got:      \"%s\"\n" DEFAULT, buffer);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s output matched (%zu bytes) as expected\n" DEFAULT,
		num, desc, len);
	return (0);
}
