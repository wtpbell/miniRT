/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_to_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:49:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 16:50:52 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	ft_stof(const char *s, float *f)
{
	float	fact;
	int		decimal;

	fact = 1;
	decimal = 0;
	*f = 0;
	if (*s == '-')
	{
		s++;
		fact = -1;
	}
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
		{
			*error() = ERR_INVALID_VALUE;
			return (false);
		}
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
	if (*s == '-')
	{
		s++;
		fact = -1;
	}
	while (*s)
	{
		if (*s >= '0' && *s <= '9')
			*i = *i * 10 + (*s - '0');
		else
		{
			*error() = ERR_INVALID_VALUE;
			return (false);
		}
		s++;
	}
	*i *= fact;
	return (true);
}
