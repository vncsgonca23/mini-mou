#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"
#include "../../../utils/constants.h"

t_list	*ft_create_elem(void *data);

int	main(void)
{
	int		error = 0;
	char	*str = "hello 42";
	t_list	*n1 = ft_create_elem(str);

	if (n1 == NULL || n1->data != str || n1->next != NULL)
	{
		printf("    " RED "[1] ft_create_elem(\"hello 42\") failed (node=%p)\n" DEFAULT, (void *)n1);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] ft_create_elem(\"hello 42\") initialized data & next=NULL\n" DEFAULT);
	free(n1);

	t_list	*n2 = ft_create_elem(NULL);
	if (n2 == NULL || n2->data != NULL || n2->next != NULL)
	{
		printf("    " RED "[2] ft_create_elem(NULL) failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] ft_create_elem(NULL) initialized data=NULL & next=NULL\n" DEFAULT);
	free(n2);

	int		val = 42;
	t_list	*n3 = ft_create_elem(&val);
	if (n3 == NULL || n3->data != &val || *(int *)n3->data != 42 || n3->next != NULL)
	{
		printf("    " RED "[3] ft_create_elem(&int) failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [3] ft_create_elem(&int) passed\n" DEFAULT);
	free(n3);

	return (error);
}
