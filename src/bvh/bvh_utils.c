/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/12 16:49:36 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 17:00:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"

void	tri_swap(t_tri *a, t_tri *b)
{
	t_tri	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	calc_tri_centroid(t_tri *triangles, int count)
{
	int		i;
	t_tri	*tri;

	i = 0;
	while (i < count)
	{
		tri = triangles + i;
		tri->centroid = v3f_scale(
				v3f_add(v3f_add(tri->v0, tri->v1), tri->v2), 1.0f / 3.0f);
		++i;
	}
}
