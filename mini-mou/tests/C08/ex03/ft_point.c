#include "../../../../ex03/ft_point.h"
#include "../../../../ex03/ft_point.h"
#include <stdio.h>
#include "../../../utils/constants.h"

void	set_point(t_point *point)
{
	point->x = 42;
	point->y = 21;
}

int	main(void)
{
	t_point	point;
	int		error = 0;

	set_point(&point);
	if (point.x != 42 || point.y != 21)
	{
		printf("    " RED "[1] set_point(&point) Expected (42, 21), got (%d, %d)\n" DEFAULT,
			point.x, point.y);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] set_point(&point) set x = 42, y = 21 as expected\n" DEFAULT);

	point.x = -100;
	point.y = 0;
	if (point.x != -100 || point.y != 0)
	{
		printf("    " RED "[2] Negative/zero assignment failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Double inclusion & field assignments verified\n" DEFAULT);

	return (error);
}
