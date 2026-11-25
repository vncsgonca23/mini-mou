#include "../../../../ex02/ft_abs.h"
#include "../../../../ex02/ft_abs.h"
#include <stdio.h>
#include <limits.h>
#include "../../../utils/constants.h"

static int	check_case(int num, const char *desc, int actual, int expected)
{
	if (actual != expected)
	{
		printf("    " RED "[%d] %s Expected %d, got %d\n" DEFAULT, num, desc, expected, actual);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [%d] %s Expected %d, got %d\n" DEFAULT, num, desc, expected, actual);
	return (0);
}

int	main(void)
{
	int	error = 0;

	error += check_case(1, "ABS(5)", ABS(5), 5);
	error += check_case(2, "ABS(-5)", ABS(-5), 5);
	error += check_case(3, "ABS(0)", ABS(0), 0);
	error += check_case(4, "ABS(1)", ABS(1), 1);
	error += check_case(5, "ABS(-1)", ABS(-1), 1);
	error += check_case(6, "ABS(INT_MAX)", ABS(INT_MAX), INT_MAX);
	error += check_case(7, "ABS(-5 + 2) (inner expression parens)", ABS(-5 + 2), 3);
	error += check_case(8, "ABS(2 - 7) (inner subtraction parens)", ABS(2 - 7), 5);
	error += check_case(9, "10 * ABS(-3) (outer parens precedence)", 10 * ABS(-3), 30);
	error += check_case(10, "ABS(-4) + ABS(-6) (combined expressions)", ABS(-4) + ABS(-6), 10);
	error += check_case(11, "-ABS(-5) (unary minus precedence)", -ABS(-5), -5);
	return (error);
}
