/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_snprintf_str.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/13 20:06:18 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 20:06:52 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "rt_snprintf.h"

void	stos(t_str *str, const char *src)
{
	size_t	len;

	len = ft_strlen(src);
	if (str->len + len >= str->cap)
		len = str->cap - str->len;
	ft_memmove(str->s + str->len, src, len);
	str->len += len;
}

void	ctos(t_str *str, char c)
{
	if (str->len + 1 >= str->cap)
		return ;
	*(str->s + str->len) = c;
	str->len += 1;
}
