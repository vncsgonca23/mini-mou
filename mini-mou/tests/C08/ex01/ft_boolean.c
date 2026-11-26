#include "../../../../ex01/ft_boolean.h"
#include "../../../../ex01/ft_boolean.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "../../../utils/constants.h"

static void	test_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

static t_bool	ft_is_even(int nbr)
{
	return ((EVEN(nbr)) ? TRUE : FALSE);
}

int	main(void)
{
	int	error = 0;

	if (TRUE == FALSE)
	{
		printf("    " RED "[1] TRUE and FALSE must be distinct\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] t_bool, TRUE and FALSE defined cleanly\n" DEFAULT);

	if (SUCCESS != 0)
	{
		printf("    " RED "[2] SUCCESS Expected 0, got %d\n" DEFAULT, SUCCESS);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] SUCCESS == 0\n" DEFAULT);

	if (ft_is_even(0) != TRUE || ft_is_even(2) != TRUE || ft_is_even(-4) != TRUE
		|| ft_is_even(1) != FALSE || ft_is_even(-3) != FALSE
		|| !EVEN(2 + 2) || EVEN(2 + 1))
	{
		printf("    " RED "[3] EVEN() macro failed on even/odd/negative/expression inputs\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] EVEN() macro passed on 0, positive, negative & expressions\n" DEFAULT);

	if (strcmp(EVEN_MSG, "I have an even number of arguments.\n") != 0)
	{
		printf("    " RED "[4] EVEN_MSG mismatch: got \"%s\"\n" DEFAULT, EVEN_MSG);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [4] EVEN_MSG matches subject exactly\n" DEFAULT);

	if (strcmp(ODD_MSG, "I have an odd number of arguments.\n") != 0)
	{
		printf("    " RED "[5] ODD_MSG mismatch: got \"%s\"\n" DEFAULT, ODD_MSG);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [5] ODD_MSG matches subject exactly\n" DEFAULT);

	/* Verify write() works via ft_boolean.h without manually including <unistd.h> */
	fflush(stdout);
	int	saved_stdout = dup(STDOUT_FILENO);
	int	fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	test_putstr(EVEN_MSG);
	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	char	buf[128] = {0};
	FILE	*fp = fopen("output.txt", "rb");
	size_t	len = fread(buf, 1, sizeof(buf) - 1, fp);
	fclose(fp);
	remove("output.txt");
	if (len != strlen(EVEN_MSG) || memcmp(buf, EVEN_MSG, len) != 0)
	{
		printf("    " RED "[6] Output via write() failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [6] Standalone write() & double inclusion verified\n" DEFAULT);

	return (error);
}
