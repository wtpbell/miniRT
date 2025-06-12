/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   field.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/06 19:14:48 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/12 13:55:29 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	is_field(const char *token, const char *field_name, const char **value)
{
	*value = ft_strchrnul(token, ':');
	if (ft_strncmp(field_name, token, (*value - token)) != 0)
		return (false);
	*value += (**value == ':');
	return (true);
}

static t_field	*get_field(t_field *fields, int count, char *token,
								const char **value)
{
	int	i;

	i = 0;
	*value = ft_strchrnul(token, ':');
	while (i < count)
	{
		if (ft_strncmp(fields[i].name, token, (*value - token)) == 0)
		{
			*value += (**value == ':');
			return (fields + i);
		}
		++i;
	}
	return (NULL);
}

static bool	validate_fields(t_field *fields, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if ((fields[i].state & (FILLED | HIDDEN)) == REQUIRED)
		{
			print_error(ERR_REQ_FIELD, "material", fields[i].name);
			return (false);
		}
		++i;
	}
	return (true);
}

bool	parse_fields(t_field *fields, int count, char **tokens)
{
	const char	*value;
	t_field		*field;

	value = NULL;
	while (*tokens)
	{
		field = get_field(fields, count, *tokens, &value);
		if (field == NULL)
			return (print_error(ERR_UNKNOWN_FIELD, "field", *tokens), false);
		if ((field->state & HIDDEN) != HIDDEN)
		{
			if ((field->type == FIELD_FLOAT
					&& !parse_float(field->val, value, field->limit, *tokens))
				|| (field->type == FIELD_INT
					&& !parse_int(field->val, value, field->limit, *tokens))
				|| (field->type == FIELD_V3F && !parse_v3f(field->val, value))
				|| (field->type == FIELD_COL && !parse_col(field->val, value)))
				return (false);
			field->state |= FILLED;
		}
		++tokens;
	}
	return (validate_fields(fields, count));
}
