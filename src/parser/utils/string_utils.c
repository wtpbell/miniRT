/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 11:34:10 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// char	*trim_whitespace(const char *line)
// {
// 	char	*trimmed;

// 	trimmed = ft_strtrim(line, " \t\n\r");
// 	if (!trimmed)
// 	{
// 		perror("Trimmed memory allocation failed");
// 		return (NULL);
// 	}
// 	return (trimmed);
// }

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
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')
		{
			while (str[i + 1] && (str[i + 1] == ' ' || str[i + 1] == '\t'
					|| str[i + 1] == '\n' || str[i + 1] == '\r'))
				i++;
			if (!prev_space && str[i + 1] && str[i + 1] != ',')
				str[pos++] = ' ';
		}
		else
		{
			str[pos++] = str[i];
			prev_space = (str[i] == ',');
		}
		i++;
	}
	str[pos] = '\0';
}

size_t	token_count_in_str(const char *str)
{
	size_t	count;
	bool	in_token;

	if (!str)
		return (0);
	count = 0;
	in_token = false;
	while (*str)
	{
		if (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
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
