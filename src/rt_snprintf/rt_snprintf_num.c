/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf_num.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/13 20:04:06 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/22 11:55:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_snprintf.h"

size_t	int_count(unsigned int n)
{
	size_t	count;

	count = (n == 0);
	while (n != 0)
	{
		n /= 10;
		++count;
	}
	return (count);
}

void	utos(t_str *str, unsigned int n, char prefix)
{
	size_t	count;
	char	*dst;

	count = int_count(n) + (prefix != '\0');
	while (str->len + count >= str->cap)
	{
		n /= 10;
		--count;
	}
	if (count == 0)
		return ;
	dst = str->s + (str->len + count - 1);
	if (n == 0)
		*dst-- = '0';
	while (n != 0)
	{
		*dst = '0' + (n % 10);
		n /= 10;
		--dst;
	}
	if (prefix != '\0')
		*dst = prefix;
	str->len += count;
}

void	itos(t_str *str, int n)
{
	if (n < 0)
		utos(str, ~n + 1u, '-');
	else
		utos(str, n, '\0');
}
