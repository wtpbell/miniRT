/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:23:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/13 17:23:07 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*preprocess_line(char *line)
{
	char	*trimmed;

	trimmed = trim_whitespace(line);
	if (!trimmed)
	{
		perror("Trimmed whitespace memory failed");
		return (NULL);
	}
	clean_spaces(trimmed);
	return (trimmed);
}

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

	first = get_first_token(processed);
	*out_type = ft_strdup(first);
	if (!first || !(*out_type))
	{
		free(first);
		return (false);
	}
	if (!check_duplicates(first, scene) || !validate_tokens(first, processed))
	{
		free(first);
		free(*out_type);
		*out_type = NULL;
		return (false);
	}
	free(first);
	return (true);
}

bool	parse_line(t_scene *scene, char *line)
{
	char	**tokens;
	char	*processed;
	char	*type;
	bool	result;

	while (*line && ft_strchr(" \f\n\r\t\v", *line))
		++line;
	if (*line == '\0')
		return (true);
	processed = preprocess_line(line);
	if (!processed)
		return (false);
	if (!get_type_and_validate(processed, &type, scene))
		return (free(processed), false);
	tokens = ft_split(processed, ' ');
	free(processed);
	if (!tokens)
	{
		free(type);
		return (false);
	}
	result = parse_scene_element(type, tokens, scene);
	return (free(type), free_tokens(tokens), result);
}
