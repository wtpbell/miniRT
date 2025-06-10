/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/06/10 14:50:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	clean_spaces(char *str)
{
	int		i;
	int		pos;
	bool	prev_space;

	i = 0;
	pos = 0;
	prev_space = true;
	while (str[i])
	{
		if (ft_strchr(" \f\n\r\t\v", str[i]))
		{
			while (str[i + 1] && ft_strchr(" \f\n\r\t\v", str[i + 1]))
				i++;
			if (!prev_space
				&& str[i + 1] && str[i + 1] != ',' && str[i + 1] != ':')
				str[pos++] = ' ';
		}
		else
		{
			str[pos++] = str[i];
			prev_space = (str[i] == ',' || str[i] == ':');
		}
		i++;
	}
	str[pos] = '\0';
}

size_t	token_count_in_str(const char *str)
{
	size_t	count;
	bool	in_token;

	count = 0;
	in_token = false;
	while (*str)
	{
		if (ft_strchr(" \f\n\r\t\v", *str))
			in_token = false;
		else if (!in_token)
		{
			count++;
			in_token = true;
		}
		str++;
	}
	return (count);
}

bool	validate_commas(const char *str)
{
	bool	prev_comma;
	int		i;

	prev_comma = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
		{
			if (prev_comma)
				return (print_error(ERR_FORMAT, "repeated comma", str), false);
			prev_comma = true;
		}
		else if (!ft_strchr(" \f\n\r\t\v", str[i]))
			prev_comma = false;
		i++;
	}
	return (true);
}

bool	ft_strarr_has(char **arr, const char *prefix)
{
	while (*arr)
	{
		if (ft_strncmp(*arr, prefix, ft_strlen(prefix)) == 0)
			return (true);
		arr++;
	}
	return (false);
}
