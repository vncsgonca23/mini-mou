#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_stock_str.h"
#include "../../../utils/constants.h"

struct s_stock_str	*ft_strs_to_tab(int ac, char **av);

static void	free_tab(t_stock_str *tab)
{
	if (!tab)
		return ;
	for (int i = 0; tab[i].str != 0; i++)
		free(tab[i].copy);
	free(tab);
}

int	main(void)
{
	int	error = 0;

	/* Test 1: Multiple strings including empty string and deep copy check */
	char		*strs1[] = {"hello", "world", "", "42", "piscine"};
	int			ac1 = 5;
	t_stock_str	*tab1 = ft_strs_to_tab(ac1, strs1);

	if (tab1 == NULL)
	{
		printf("    " RED "[1] ft_strs_to_tab(5, strs) returned NULL\n" DEFAULT);
		return (-1);
	}
	int	ok1 = 1;
	for (int i = 0; i < ac1; i++)
	{
		if (tab1[i].size != (int)strlen(strs1[i])
			|| tab1[i].str != strs1[i]
			|| tab1[i].copy == NULL
			|| tab1[i].copy == tab1[i].str
			|| strcmp(tab1[i].copy, strs1[i]) != 0)
		{
			printf("    " RED "[1] Entry %d mismatch (size=%d, str=%p, copy=%p)\n" DEFAULT,
				i, tab1[i].size, (void *)tab1[i].str, (void *)tab1[i].copy);
			ok1 = 0;
			error -= 1;
			break ;
		}
	}
	if (ok1 && tab1[ac1].str != 0)
	{
		printf("    " RED "[1] Last element str is not NULL sentinel\n" DEFAULT);
		ok1 = 0;
		error -= 1;
	}
	if (ok1)
	{
		/* Mutate copy and ensure original str is untouched */
		tab1[0].copy[0] = 'H';
		if (tab1[0].str[0] != 'h')
		{
			printf("    " RED "[1] Mutating copy modified original str (shallow copy)\n" DEFAULT);
			error -= 1;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [1] 5 strings (incl. empty) + deep copy + NULL sentinel verified\n" DEFAULT);
	}
	free_tab(tab1);

	/* Test 2: ac == 0 */
	char		*strs2[] = {NULL};
	t_stock_str	*tab2 = ft_strs_to_tab(0, strs2);
	if (tab2 == NULL || tab2[0].str != 0)
	{
		printf("    " RED "[2] ft_strs_to_tab(0, av) failed sentinel check\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] ac == 0 returned valid sentinel-terminated array\n" DEFAULT);
	free_tab(tab2);

	/* Test 3: Single empty string */
	char		*strs3[] = {""};
	t_stock_str	*tab3 = ft_strs_to_tab(1, strs3);
	if (tab3 == NULL || tab3[0].size != 0 || tab3[0].str != strs3[0]
		|| tab3[0].copy == NULL || tab3[0].copy == strs3[0]
		|| strcmp(tab3[0].copy, "") != 0 || tab3[1].str != 0)
	{
		printf("    " RED "[3] Single empty string test failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] Single empty string test passed\n" DEFAULT);
	free_tab(tab3);

	return (error);
}
