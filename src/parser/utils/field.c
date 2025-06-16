/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   field.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/06 19:14:48 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/16 11:29:59 by jboon         ########   odam.nl         */
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
		if ((fields[i].state & (FILLED | REQUIRED | HIDDEN)) == REQUIRED)
		{
			print_error(ERR_REQ_FIELD, "material", fields[i].name);
			return (false);
		}
		++i;
	}
	return (true);
}

bool	parse_enum(int *val, const char *str, const char *token, conv_to_enum to_enum)
{
	if (to_enum(val, str))
		return (true);
	print_error(ERR_INVALID_VALUE, token, str);
	return (false);
}

bool	parse_fields(t_field *fields, int count, char **tokens, char ***rem_tokens)
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
				|| (field->type == FIELD_COL && !parse_col(field->val, value))
				|| (field->type == FIELD_ENUM && !parse_enum(field->val, value, *tokens, field->to_enum)))
				return (false);
			field->state |= FILLED;
		}
		++tokens;
	}
	if (rem_tokens != NULL)
		*rem_tokens = tokens;
	return (validate_fields(fields, count));
}
