#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../../../ex00/ft_putchar.c"
#include "../../../utils/constants.h"

typedef struct s_test
{
	char	*desc;
	char	c;
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test	tests[] = {
		{.desc = "ft_putchar('c')", .c = 'c'},
		{.desc = "ft_putchar('x')", .c = 'x'},
		{.desc = "ft_putchar('0')", .c = '0'},
		{.desc = "ft_putchar('Z')", .c = 'Z'},
		{.desc = "ft_putchar(' ')", .c = ' '},
		{.desc = "ft_putchar('\\n')", .c = '\n'},
		{.desc = "ft_putchar('\\t')", .c = '\t'},
		{.desc = "ft_putchar('\\0')", .c = '\0'},
		{.desc = "ft_putchar('\\x7F')", .c = '\x7F'},
		{.desc = "ft_putchar('\\x80')", .c = '\x80'},
		{.desc = "ft_putchar('\\xFF')", .c = '\xFF'},
	};
	int		count = sizeof(tests) / sizeof(tests[0]);

	return (run_tests(tests, count));
}

int	run_tests(t_test *tests, int count)
{
	int		i;
	int		error = 0;

	for (i = 0; i < count; i++)
	{
		fflush(stdout);

		char	buffer[64];
		memset(buffer, 0, sizeof(buffer));

		int		saved_stdout = dup(STDOUT_FILENO);
		int		output_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);

		ft_putchar(tests[i].c);

		fflush(stdout);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);

		FILE	*fp = fopen("output.txt", "rb");
		size_t	len = fread(buffer, 1, sizeof(buffer), fp);
		fclose(fp);
		remove("output.txt");

		if (len != 1 || buffer[0] != tests[i].c)
		{
			printf("    " RED "[%d] %s Expected 1 byte (0x%02x), got %zu byte(s) (0x%02x)\n" DEFAULT,
				i + 1, tests[i].desc, (unsigned char)tests[i].c, len,
				len > 0 ? (unsigned char)buffer[0] : 0);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s output matched as expected\n" DEFAULT,
				i + 1, tests[i].desc);
	}
	return (error);
}
