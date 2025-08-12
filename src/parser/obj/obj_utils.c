/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 19:19:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 17:23:38 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	assign_v3f(t_v3f *dst[3], t_vector *cont, int *indices, int curr)
{
	int	i;

	while (curr < MAX_VERT_PER_SET)
	{
		i = indices[curr] - 1;
		if (vector_get(cont, i) == NULL)
			return (false);
		*dst[curr / 3] = *((t_v3f *)cont->items[i]);
		curr += 3;
	}
	return (true);
}

void	set_normal(t_tri *tri)
{
	t_v3f	norm;

	norm = v3f_norm(
			v3f_cross(v3f_sub(tri->v1, tri->v0), v3f_sub(tri->v2, tri->v0)));
	tri->vn0 = norm;
	tri->vn1 = norm;
	tri->vn2 = norm;
}
