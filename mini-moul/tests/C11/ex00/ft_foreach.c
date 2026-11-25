#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../ex00/ft_foreach.c"
#include "../../../utils/constants.h"

static int	g_log[32];
static int	g_idx;

static void	record_val(int n)
{
	if (g_idx < 32)
		g_log[g_idx++] = n;
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Standard array in exact order */
	int	tab1[] = {10, -20, 0, 42, 1337};
	g_idx = 0;
	ft_foreach(tab1, 5, &record_val);
	if (g_idx != 5 || memcmp(g_log, tab1, sizeof(tab1)) != 0)
	{
		printf("    " RED "[1] Standard 5-element array order mismatch (called %d times)\n" DEFAULT, g_idx);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] Standard 5-element array applied in order\n" DEFAULT);

	/* Test 2: Empty array (length == 0) */
	g_idx = 0;
	ft_foreach(tab1, 0, &record_val);
	if (g_idx != 0)
	{
		printf("    " RED "[2] length == 0 called f %d times (expected 0)\n" DEFAULT, g_idx);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] length == 0 did not invoke callback\n" DEFAULT);

	/* Test 3: Single element */
	int	tab3[] = {-99};
	g_idx = 0;
	ft_foreach(tab3, 1, &record_val);
	if (g_idx != 1 || g_log[0] != -99)
	{
		printf("    " RED "[3] Single element test failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Single element applied correctly\n" DEFAULT);

	return (error);
}
