/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_vertex.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 18:06:33 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/08 17:09:26 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "vector.h"
#include "container.h"

static int	to_v3f(t_v3f *dst, char *saveptr, t_v2f lim)
{
	int		count;
	char	*token;

	count = 0;
	while (true)
	{
		token = rt_strtok(NULL, WHITESPACE, &saveptr);
		if (token == NULL)
			break ;
		if (count < 3 && !parse_float(dst->_axis + count, token, lim, token))
			return (0);
		++count;
	}
	return (count);
}

static bool	copy_v3f_to(const t_v3f point, t_vector *v)
{
	t_v3f	*copy;

	copy = malloc(sizeof(t_v3f));
	if (copy == NULL)
		return (perror("minirt"), false);
	*copy = point;
	if (vector_add(v, copy))
		return (true);
	return (perror("minirt"), free(copy), false);
}

bool	parse_vertex(char *str, t_vector *v, const t_vert_ran *ran)
{
	int		count;
	t_v3f	point;

	count = to_v3f(&point, str, ran->lim);
	if (count < (int)ran->comp.x || count > (int)ran->comp.y)
		return (print_error(ERR_OBJ_VERT_COMP, "obj", NULL), false);
	while (count < (int)ran->comp.y)
	{
		point._axis[count] = ran->def._axis[count];
		++count;
	}
	return (copy_v3f_to(point, v));
}
