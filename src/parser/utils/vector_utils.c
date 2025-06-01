/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:24:01 by bewong            #+#    #+#             */
/*   Updated: 2025/06/01 17:03:24 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse_floats(const char **tokens, float *out,
					size_t count, const char *ctx)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (!ft_stof(tokens[i], &out[i]))
		{
			print_error(ERR_STOF, ctx, tokens[i]);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	parse_ints(const char **tokens, int *out,
					size_t count, const char *ctx)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (!ft_stoi(tokens[i], &out[i]))
		{
			print_error(ERR_STOI, ctx, tokens[i]);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	parse_v3f(t_v3f *v3f, const char *str)
{
	char	**tokens;
	float	values[3];

	if (!split_and_validate(str, &tokens, 3, "V3F"))
		return (false);
	if (!parse_floats((const char **)tokens, values, 3, "parse v3f"))
		return (free_tokens(tokens), false);
	*v3f = (t_v3f){{values[0], values[1], values[2]}};
	free_tokens(tokens);
	return (true);
}

bool	parse_col(t_v3f *color, const char *str)
{
	char	**tokens;
	int		rgb[3];
	int		i;

	i = 0;
	if (!split_and_validate(str, &tokens, 3, "parse color"))
		return (false);
	if (!parse_ints((const char **)tokens, rgb, 3, "parse color"))
		return (free_tokens(tokens), false);
	while (i < 3)
	{
		if (rgb[i] < MIN_COLOR || rgb[i] > MAX_COLOR)
		{
			print_error(ERR_RANGE, "color 0-255", tokens[i]);
			return (free_tokens(tokens), false);
		}
		i++;
	}
	*color = (t_v3f){{
		.x = (float)rgb[0] / 255.0f,
		.y = (float)rgb[1] / 255.0f,
		.z = (float)rgb[2] / 255.0f
	}};
	free_tokens(tokens);
	return (true);
}

bool	parse_dir(t_v3f *dir, const char *str)
{
	char	**tokens;
	float	values[3];
	int		i;

	i = 0;
	if (!split_and_validate(str, &tokens, 3, "parse dir"))
		return (false);
	if (!parse_floats((const char **)tokens, values, 3, "parse dir"))
		return (free_tokens(tokens), false);
	while (i < 3)
	{
		if (values[i] < MIN_DIR || values[i] > MAX_DIR)
		{
			print_error(ERR_RANGE, "orientation -1,1", tokens[i]);
			return (free_tokens(tokens), false);
		}
		i++;
	}
	*dir = v3f_norm((t_v3f){{values[0], values[1], values[2]}});
	free_tokens(tokens);
	return (true);
}
