/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/15 18:03:06 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static bool	get_type_and_validate(char *processed, char *out_type)
{
	if (!validate_tokens(out_type, processed))
		return (false);
	return (true);
}

bool	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	bool	result;

	while (*line && ft_strchr(" \f\n\r\t\v", *line))
		++line;
	if (*line == '\0' || *line == '#')
		return (true);
	clean_spaces(line);
	if (!validate_commas(line))
		return (false);
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (perror("parse_line"), false);
	if (!get_type_and_validate(line, *tokens))
		return (free_tokens(tokens), false);
	result = parse_scene_element(*tokens, tokens, scene);
	free_tokens(tokens);
	return (result);
}
