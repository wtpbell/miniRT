/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 15:32:11 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*trim_whitespace(const char *line)
{
	char	*trimmed;

	trimmed = ft_strtrim(line, " \t\n\r");
	if (!trimmed)
	{
		*error() = ERR_MEM;
		return (NULL);
	}
	return (trimmed);
}


static void	process_char(const char *str, char *result, int *i, int *write,
		bool *prev_space)
{
	if ((str[*i] == ' ' || str[*i] == '\t'))
	{
		while (str[*i + 1] && (str[*i + 1] == ' ' || str[*i + 1] == '\t'))
			(*i)++;
		if (!*prev_space && str[*i + 1] && str[*i + 1] != ',')
			result[(*write)++] = ' ';
	}
	else
	{
		result[(*write)++] = str[*i];
		*prev_space = (str[*i] == ',');
	}
	(*i)++;
}

char	*clean_spaces(const char *str)
{
	char	*result;
	int		i;
	int		write;
	bool	prev_space;

	result = ft_strdup(str);
	if (!result)
		return (NULL);
	i = 0;
	write = 0;
	prev_space = true;
	while (str[i])
		process_char(str, result, &i, &write, &prev_space);
	result[write] = '\0';
	return (result);
}

size_t	count_tokens_in_str(const char *str)
{
	size_t	count;
	bool	in_token;

	if (!str)
		return (0);
	count = 0;
	in_token = false;
	while (*str)
	{
		if (*str == ' ' || *str == '\t' || *str == '\n')
			in_token = false;
		else if (!in_token)
		{
			in_token = true;
			count++;
		}
		str++;
	}
	return (count);
}


