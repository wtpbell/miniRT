/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/15 12:05:02 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	get_type_and_validate(char *processed, char **out_type)
{
	if (!validate_tokens(*out_type, processed))
		return (false);
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
	clean_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (perror("Split failed"), false);
	type = tokens[0];
	if (!get_type_and_validate(line, &type))
		return (free_tokens(tokens), false);
	result = parse_scene_element(type, tokens, scene);
	free_tokens(tokens);
	return (result);
}
