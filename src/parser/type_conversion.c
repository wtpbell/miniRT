/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_conversion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:00:12 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 16:00:12 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <limits.h>

bool	ft_stoi(const char *s, int *i)
{
	int	multipler;

	multipler = 1;
	*i = 0;
	if (*s == '-')
	{
		s++;
		multipler = -1;
	}
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			*i = *i * 10 + (*s - '0');
		else
			return (false);
		s++;
	}
	*i *= multipler;
	return (true);
}

static bool	ft_stof2(const char *s, float *res, float *multipler)
{
	int	point_seen;
	int	i;

	point_seen = 0;
	while (*s)
	{
		if (*s == '.')
			point_seen++;
		else
		{
			i = *s - '0';
			if (i >= 0 && i <= 9)
			{
				if (point_seen)
					*multipler /= 10.0f;
				*res = *res * 10.0f + (float)i;
			}
			else
				return (false);
		}
		s++;
	}
	return (true);
}

bool	ft_stof(const char *s, float *f)
{
	float	multipler;

	multipler = 1;
	*f = 0.0f;
	if (*s == '-')
	{
		s++;
		multipler = -1;
	}
	if (ft_stof2(s, f, &multipler) == false)
	{
		*error() = ERR_STOF;
		return (false);
	}
	*f *= multipler;
	return (true);
}
