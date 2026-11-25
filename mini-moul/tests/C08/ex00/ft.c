#include "../../../../ex00/ft.h"
#include "../../../../ex00/ft.h"
#include <stdio.h>
#include "../../../utils/constants.h"

void	ft_putchar(char c)
{
	(void)c;
}

void	ft_swap(int *a, int *b)
{
	int	tmp = *a;

	*a = *b;
	*b = tmp;
}

void	ft_putstr(char *str)
{
	(void)str;
}

int	ft_strlen(char *str)
{
	int	len = 0;

	while (str[len])
		len++;
	return (len);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	main(void)
{
	void	(*p_putchar)(char) = ft_putchar;
	void	(*p_swap)(int *, int *) = ft_swap;
	void	(*p_putstr)(char *) = ft_putstr;
	int		(*p_strlen)(char *) = ft_strlen;
	int		(*p_strcmp)(char *, char *) = ft_strcmp;
	int		a = 1;
	int		b = 2;

	p_putchar('a');
	p_swap(&a, &b);
	p_putstr("42");
	if (a != 2 || b != 1 || p_strlen("42") != 2 || p_strcmp("ab", "ab") != 0)
	{
		printf("    " RED "[1] Prototype verification failed\n" DEFAULT);
		return (-1);
	}
	printf("  " GREEN CHECKMARK GREY " [1] Double inclusion & all 5 exact prototypes verified\n" DEFAULT);
	return (0);
}
