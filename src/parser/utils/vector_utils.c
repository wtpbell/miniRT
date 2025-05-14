/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 10:43:10 by bewong           ###   ########.fr       */
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

	count = 1;
	while (*str)
	{
		if (*str != ',')
			++count;
		++str;
	}
	return (count);
}

bool	parse_diameter(const char *str, float *out)
{
	float	value;

	if (!ft_stof(str, &value))
	{
		print_error(ERR_STOF, "parse diameter", str);
		return (false);
	}
	if (value < MIN_RADIUS || value > MAX_RADIUS)
	{
		print_error(ERR_RANGE, "parse diameter", str);
		return (false);
	}
	*out = value;
	return (true);
}

bool	parse_v3f(t_v3f *v3f, const char *str)
{
	char	**tokens;
	bool	result;

	tokens = ft_split(str, ',');
	if (!tokens)
		return (perror("Parse_v3f memory failed"), false);
	result = true;
	if (token_count(tokens) == 3)
	{
		ft_bzero(v3f, sizeof(t_v3f));
		result = (
				ft_stof(tokens[0], &v3f->x)
				&& ft_stof(tokens[1], &v3f->y)
				&& ft_stof(tokens[2], &v3f->z)
				);
	}
	else
	{
		print_error(ERR_V3F, "parse v3f", *tokens);
		result = false;
	}
	free_tokens(tokens);
	return (result);
}

bool	parse_col(t_col32 *col, const char *str)
{
	char	**tokens;
	int		rgb[3];
	int		i;

	tokens = ft_split(str, ',');
	if (!tokens)
		return (false);
	if (token_count(tokens) != 3)
		return (free_tokens(tokens), false);
	i = 0;
	while (i < 3)
	{
		if (!ft_stoi(tokens[i], &rgb[i]))
			return (print_error(ERR_STOI, "parse color", tokens[i]),
				free_tokens(tokens), false);
		if (rgb[i] < MIN_COLOR || rgb[i] > MAX_COLOR)
			return (print_error(ERR_RANGE, "color range 0-255", tokens[i]),
				free_tokens(tokens), false);
		i++;
	}
	*col = init_col32(rgb[0], rgb[1], rgb[2], 0xFF);
	free_tokens(tokens);
	return (true);
}
