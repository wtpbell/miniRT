/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:23:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 18:14:34 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char *preprocess_line(char *line)
{
	char	*trimmed;
	char	*processed;

	trimmed = trim_whitespace(line);
	if (!trimmed)
		return (NULL);
	printf("After trim: '%s'\n", trimmed);
	processed = clean_spaces(trimmed);
	printf("After clean: '%s'\n", processed);
	free(trimmed);
	if (processed)
		printf("After preprocessing: '%s'\n", processed);
	return (processed);
}

static size_t	get_expected_token_count(const char *type)
{
	if (ft_strcmp(type, "sp") == 0)
		return (4);
	if (ft_strcmp(type, "pl") == 0)
		return (4);
	if (ft_strcmp(type, "cy") == 0)
		return (6);
	if (ft_strcmp(type, "A") == 0)
		return (3);
	if (ft_strcmp(type, "C") == 0)
		return (4);
	if (ft_strcmp(type, "L") == 0)
		return (4);
	return (0);
}

static char *get_first_token(const char *str)
{
	const char	*start;
	const char	*end;
	char		*token;

	while (*str == ' ' || *str == '\t')
		str++;
	start = str;
	while (*str && *str != ' ' && *str != '\t')
		str++;
	end = str;
	token = ft_substr(start, 0, end - start);
	return (token);
}

static bool	validate_tokens(const char *first_token, const char *line)
{
	size_t	expected;
	size_t	count;

	expected = get_expected_token_count(first_token);
	if (expected == 0)
	{
		*error() = ERR_UNKNOWN_TOKEN;
		return (false);
	}
	count = count_tokens_in_str(line);
	if (count != expected)
	{
		*error() = ERR_TOKEN_COUNT;
		return (false);
	}
	return (true);
}

static bool	check_duplicates(const char *type, t_scene *scene)
{
	if ((ft_strcmp(type, "sp") == 0 || ft_strcmp(type, "pl") == 0 ||
		ft_strcmp(type, "cy") == 0) && has_duplicate_identifier(type, scene))
	{
		*error() = ERR_DUPLICATE;
		return (false);
	}
	return (true);
}

static bool	get_type_and_validate(char *processed, char **out_type, t_scene *scene)
{
	char	*first;

	first = get_first_token(processed);
	if (!first || !(*out_type = ft_strdup(first)))
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

	processed = preprocess_line(line);
	if (!processed)
		return (false);
	printf("After preprocessing: '%s'\n", processed);
	if (!get_type_and_validate(processed, &type, scene))
		return (free(processed), false);
	tokens = ft_split(processed, ' ');
	free(processed);
	if (!tokens)
	{
		free(type);
		*error() = ERR_MEM;
		return (false);
	}
	result = parse_scene_element(type, tokens, scene);
	free(type);
	free_tokens(tokens);
	return (result);
}
