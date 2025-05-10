/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:48:36 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 14:48:36 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	token_count(char **tokens)
{
	size_t	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

bool	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool	parse_v3f(t_v3f *v3f, const char *str)
{
	char	**tokens;
	bool	result;

	if (!str || !v3f)
		return (false);
	printf("Parsing v3f from: '%s'\n", str);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	result = true;
	if (token_count(tokens) == 3)
	{
		result = (
			ft_stof(tokens[0], &v3f->x) &&
			ft_stof(tokens[1], &v3f->y) &&
			ft_stof(tokens[2], &v3f->z)
			);
	}
	else
		result = false;
	free_tokens(tokens);
	return (result);
}

bool	parse_float(float *f, const char *str)
{
	printf("Parsing float from: '%s'\n", str);
	if (!str || !f)
		return (false);
	while (ft_isspace(*str))
		str++;
	if (!ft_stof(str, f))
	{
		*error() = ERR_STOF;
		return (false);
	}
	return (true);
}

bool	parse_col(t_col32 *col, const char *str)
{
	char	**tokens;
	int		rgb[3];
	int		i;
	char	*trimmed;

	printf("Parsing color from: '%s'\n", str);
	if (!str || !col)
		return (false);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	if (token_count(tokens) != 3)
		return (free_tokens(tokens), false);
	i = 0;
	while (i < 3)
	{
		trimmed = ft_strtrim(tokens[i], " \t\n\r");
		if (!trimmed)
			return (free_tokens(tokens), false);
		if (!ft_stoi(trimmed, &rgb[i]))
			return (free(trimmed), free_tokens(tokens), false);
		free(trimmed);
		if (rgb[i] < 0 || rgb[i] > 255)
			return (free_tokens(tokens), false);
		i++;
	}
	*col = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF;
	free_tokens(tokens);
	return (true);
}
