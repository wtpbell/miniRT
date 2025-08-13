/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_strtok.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/03 12:06:29 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 09:41:20 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*rt_strtok(char *str, const char *delim, char **saveptr)
{
	char	*start;

	if (str == NULL)
		str = *saveptr;
	while (*str && ft_strchr(delim, *str))
		++str;
	if (*str == '\0')
		return (NULL);
	start = str;
	while (*str && !ft_strchr(delim, *str))
		++str;
	*saveptr = str + (*str != '\0');
	*str = '\0';
	return (start);
}

size_t	rt_count_occ(const char *str, char c)
{
	size_t	occ;

	occ = 0;
	while (*str)
	{
		if (*str == c)
			++occ;
		++str;
	}
	return (occ);
}
