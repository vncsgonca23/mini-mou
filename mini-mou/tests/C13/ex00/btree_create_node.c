#include <stdio.h>
#include <stdlib.h>
#include "ft_btree.h"
#include "../../../utils/constants.h"

t_btree	*btree_create_node(void *item);

int	main(void)
{
	int		error = 0;
	char	*val = "root";
	t_btree	*n1 = btree_create_node(val);

	if (n1 == NULL || n1->item != val || n1->left != NULL || n1->right != NULL)
	{
		printf("    " RED "[1] btree_create_node(\"root\") failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [1] btree_create_node(\"root\") set item & left=right=NULL\n" DEFAULT);
	free(n1);

	t_btree	*n2 = btree_create_node(NULL);
	if (n2 == NULL || n2->item != NULL || n2->left != NULL || n2->right != NULL)
	{
		printf("    " RED "[2] btree_create_node(NULL) failed\n" DEFAULT);
		error -= 1;
	}
	else
		printf("  " GREEN CHECKMARK GREY " [2] btree_create_node(NULL) passed\n" DEFAULT);
	free(n2);

	return (error);
}
