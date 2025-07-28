/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   field.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/06 19:14:48 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/28 10:02:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_field	init_field(const char *name, void *mem, t_f_type type, t_v2f lim)
{
	return ((t_field){name, mem, type, lim, EMPTY, {NULL}});
}

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
		if (ft_strncmp(fields[i].name, token, (*value - token)) == 0
			&& *(fields[i].name + (*value - token)) == '\0')
		{
			*value += (**value == ':');
			return (fields + i);
		}
		++i;
	}
	return (NULL);
}

bool	parse_enum(int *val, const char *str, const char *token,
						t_conv_to_enum to_enum)
{
	if (to_enum(val, str))
		return (true);
	print_error(ERR_INVALID_VALUE, token, str);
	return (false);
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
			if ((field->type == FIELD_FLT
					&& !parse_float(field->val, value, field->limit, *tokens))
				|| (field->type == FIELD_INT
					&& !parse_int(field->val, value, field->limit, *tokens))
				|| (field->type == FIELD_V3F && !parse_v3f(field->val, value))
				|| (field->type == FIELD_COL && !parse_col(field->val, value))
				|| (field->type == FIELD_ENUM
					&& !parse_enum(field->val, value, *tokens, field->to_enum)))
				return (false);
			field->state |= FILLED;
		}
		++tokens;
	}
	return (true);
}
