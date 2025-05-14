/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 10:42:14 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	check_duplicates(const char *type, t_scene *scene)
{
	if ((ft_strcmp(type, "A") == 0 || ft_strcmp(type, "C") == 0
			|| ft_strcmp(type, "L") == 0)
		&& has_duplicate_identifier(type, scene))
	{
		print_error(ERR_DUPLICATE, "parse", type);
		return (false);
	}
	return (true);
}

static bool	get_type_and_validate(char *processed,
			char **out_type, t_scene *scene)
{
	char	*first;

	*out_type = NULL;
	first = get_first_token(processed);
	if (!first)
		return (false);
	*out_type = first;
	if (!check_duplicates(first, scene) || !validate_tokens(first, processed))
	{
		free(first);
		return (false);
	}
	*out_type = first;
	return (true);
}

bool	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	char	*type;
	bool	result;

	while (*line && ft_strchr(" \f\n\r\t\v", *line))
		++line;
	if (*line == '\0' || *line == '#')
		return (true);
	tokens = ft_split(line, ' ');
	if (!tokens)
	{
		perror("Split failed");
		return (false);
	}
	type = tokens[0];
	if (!get_type_and_validate(type, &type, scene))
		return (free_tokens(tokens), false);
	result = parse_scene_element(type, tokens, scene);
	return (free(type), free_tokens(tokens), result);
}
