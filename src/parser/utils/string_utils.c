/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/01 18:53:15 by jboon         ########   odam.nl         */
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

bool	str_is_empty(const char *s)
{
	return (s == NULL || *s == '\0');
}
