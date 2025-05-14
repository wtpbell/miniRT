/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 13:51:40 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

	tokens = ft_split(str, ',');
	if (!tokens)
		return (perror("Parse_v3f memory failed"), false);
	if (token_count(tokens) != 3)
	{
		print_error(ERR_V3F, "parse v3f", *tokens);
		free_tokens(tokens);
		return (false);
	}
	ft_bzero(v3f, sizeof(t_v3f));
	if (!ft_stof(tokens[0], &v3f->x) || !ft_stof(tokens[1], &v3f->y)
		|| !ft_stof(tokens[2], &v3f->z))
		return (free_tokens(tokens), false);
	free_tokens(tokens);
	return (true);
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

static bool	validate_and_norm_dir(t_v3f *dir, const char *str)
{
	if (!v3f_dir_valid(dir))
	{
		print_error(ERR_RANGE, "direction range -1.0f - 1.0f", str);
		return (false);
	}
	*dir = v3f_norm(*dir);
	return (true);
}

bool	parse_dir(t_v3f *dir, const char *str)
{
	char	**tokens;

	tokens = ft_split(str, ',');
	if (!tokens)
		return (perror("Parse_dir memory failed"), false);
	if (token_count(tokens) != 3)
	{
		print_error(ERR_V3F, "parse dir", *tokens);
		free_tokens(tokens);
		return (false);
	}
	ft_bzero(dir, sizeof(t_v3f));
	if (!ft_stof(tokens[0], &dir->x) || !ft_stof(tokens[1], &dir->y)
		|| !ft_stof(tokens[2], &dir->z) || !validate_and_norm_dir(dir, str))
		return (free_tokens(tokens), false);
	free_tokens(tokens);
	return (true);
}
