/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_to_num.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:49:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 19:06:29 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*

// TODO check this or cap the addition
#include <stdbool.h>
#include <math.h>

isnan(f);
isinf(f);
*/
bool	ft_stof(const char *s, float *f)
{
	float	fact;
	int		decimal;

	fact = 1;
	decimal = 0;
	*f = 0;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			fact = -1;
		s++;
	}
	if (!*s)
		return (*error() = ERR_STOF, false);
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
		{
			if (decimal)
				fact /= 10;
			*f = *f * 10 + (*s - '0');
		}
		else if (*s == '.' && !decimal)
			decimal = 1;
		else
			return (*error() = ERR_STOF, false);
		s++;
	}
	*f *= fact;
	return (true);
}

bool	ft_stoi(const char *s, int *i)
{
	int	fact;

	fact = 1;
	*i = 0;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			fact = -1;
		s++;
	}
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			*i = *i * 10 + (*s - '0');
		else
		{
			*error() = ERR_STOI;
			return (false);
		}
		s++;
	}
	*i *= fact;
	return (true);
}
