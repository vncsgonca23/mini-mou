#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex01/ft_map.c"
#include "../../../utils/constants.h"

static int	g_order[32];
static int	g_idx;

static int	double_and_log(int n)
{
	if (g_idx < 32)
		g_order[g_idx++] = n;
	return (n * 2);
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Standard array + verify order + verify original array untouched */
	int	tab1[] = {-5, 0, 1, 21, 100};
	int	exp1[] = {-10, 0, 2, 42, 200};
	g_idx = 0;
	int	*res1 = ft_map(tab1, 5, &double_and_log);
	if (res1 == NULL || res1 == tab1 || g_idx != 5
		|| memcmp(g_order, tab1, sizeof(tab1)) != 0
		|| memcmp(res1, exp1, sizeof(exp1)) != 0)
	{
		printf("    " RED "[1] Standard 5-element map failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Standard 5-element map (in order, new allocation) passed\n" DEFAULT);
	free(res1);

	/* Test 2: Single element */
	int	tab2[] = {21};
	g_idx = 0;
	int	*res2 = ft_map(tab2, 1, &double_and_log);
	if (res2 == NULL || res2[0] != 42 || g_idx != 1)
	{
		printf("    " RED "[2] Single element map failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] Single element map passed\n" DEFAULT);
	free(res2);

	/* Test 3: length == 0 */
	g_idx = 0;
	int	*res3 = ft_map(tab1, 0, &double_and_log);
	if (g_idx != 0)
	{
		printf("    " RED "[3] length == 0 invoked callback %d times\n" DEFAULT, g_idx);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] length == 0 did not invoke callback\n" DEFAULT);
	free(res3);

	return (error);
}
