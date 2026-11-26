#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex11/ft_putstr_non_printable.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	*src;
	char	*expected;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	unsigned char	extended_bytes[0x80 + 1] = {0};
	char			extended_escapes[0x80 * 3 + 1];

	for (unsigned int byte = 0x80; byte <= 0xff; byte++)
	{
		unsigned int index = byte - 0x80;

		extended_bytes[index] = (unsigned char)byte;
		snprintf(extended_escapes + index * 3, 4, "\\%02x", byte);
	}

	t_test	tests[] = {
		{
			.desc = "Subject example (Hello\\nHow are you?)",
			.src = "Hello\nHow are you?",
			.expected = "Hello\\0aHow are you?",
		},
		{
			.desc = "Empty string",
			.src = "",
			.expected = "",
		},
		{
			.desc = "Printable ASCII boundaries (32..126)",
			.src = " !~09AZaz",
			.expected = " !~09AZaz",
		},
		{
			.desc = "Low control chars (0x01, 0x09, 0x0d, 0x1f)",
			.src = "\x01\t\r\x1f",
			.expected = "\\01\\09\\0d\\1f",
		},
		{
			.desc = "DEL (0x7f) and extended bytes (0x80, 0x9a, 0xff)",
			.src = "A\x7f\x80\x9a\xff" "Z",
			.expected = "A\\7f\\80\\9a\\ffZ",
		},
		{
			.desc = "String starts with extended bytes, then mixes ASCII and controls",
			.src = "\x9b\x85\xc6\xae" "X0" "\xbb" "N" "\x06\xa7\xa9\x0e",
			.expected = "\\9b\\85\\c6\\aeX0\\bbN\\06\\a7\\a9\\0e",
		},
		{
			.desc = "All bytes from 0x80 through 0xff are non-printable",
			.src = (char *)extended_bytes,
			.expected = extended_escapes,
		},
		{
			.desc = "Mixed printable and non-printable text",
			.src = "Some\nrandom\ttext\x0Fwith\nnon-printable\rcharacters",
			.expected = "Some\\0arandom\\09text\\0fwith\\0anon-printable\\0dcharacters",
		},
		{
			.desc = "String with embedded NUL stops at NUL",
			.src = "Hi\n\0hidden\t",
			.expected = "Hi\\0a",
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

		ft_putstr_non_printable(tests[i].src);

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
