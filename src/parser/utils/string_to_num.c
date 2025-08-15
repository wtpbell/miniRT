/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_to_num.c                                    :+:    :+:            */
/*                                                     ft                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:49:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/14 21:15:00 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <math.h>
#include "rt_math.h"
#include "rt_types.h"
#include "rt_snprintf.h"

static bool	parse_number(const char *s, float *f, float *fact)
{
	int		decimal;

	decimal = 0;
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
		{
			if (decimal)
				*fact /= 10.0f;
			*f = *f * 10.0f + (*s - '0');
		}
		else if (*s == '.' && !decimal)
			decimal = 1;
		else
			return (false);
		s++;
	}
	return (true);
}

bool	ft_stof(const char *s, float *f)
{
	float	fact;

	fact = 1.0f;
	*f = 0.0f;
	if (ft_strstr(s, "inf") || ft_strstr(s, "nan") || ft_strchr(s, 'e'))
		return (false);
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			fact = -1.0f;
		s++;
	}
	if (!*s || !parse_number(s, f, &fact))
		return (false);
	*f *= fact;
	if (isinf(*f) || isnan(*f))
		return (false);
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
			return (false);
		s++;
	}
	*i *= fact;
	return (true);
}
