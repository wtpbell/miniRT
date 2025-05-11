/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 18:11:24 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static size_t	token_count(char **tokens)
{
	size_t	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

size_t	count_vector_components(const char *str)
{
	size_t	count;
	size_t	i;

	count = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			count++;
		i++;
	}
	return (count);
}

bool	parse_float(const char *str, float *out)
{
	return (ft_stof(str, out));
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

bool	parse_col(t_col32 *col, const char *str)
{
	char	**tokens;
	int		rgb[3];
	int		i;

	printf("Parsing color from: '%s'\n", str);
	if (!str || !col)
		return (false);
	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	printf("Token count: %zu\n", token_count(tokens));
	if (token_count(tokens) != 3)
		return (free_tokens(tokens), false);
	i = 0;
	while (i < 3)
	{
		printf("Parsing token %d: '%s'\n", i, tokens[i]);
		if (!ft_stoi(tokens[i], &rgb[i]))
			return (free_tokens(tokens), false);
		printf("Value for token %d: %d\n", i, rgb[i]);
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			*error() = ERR_RANGE;
			return (free_tokens(tokens), false);
		}
		i++;
	}
	*col = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 0xFF;
	free_tokens(tokens);
	return (true);
}

