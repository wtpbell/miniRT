/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   triangle.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 12:00:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/09 09:23:41 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"

static inline t_v3f	get_mid_point(t_v3f v0, t_v3f v1, t_v3f v2)
{
	return (v3f_scale(v3f_add(v3f_add(v0, v1), v2), 1.0f / 3.0f));
}

static inline t_v3f	get_normal(t_v3f v0, t_v3f v1, t_v3f v2)
{
	return (v3f_norm(v3f_cross(v3f_sub(v1, v0), v3f_sub(v2, v0))));
}

bool	parse_triangle(char **tokens, t_scene *scene)
{
	t_obj	*obj;
	t_tri	tri;
	t_v3f	color;

	if (!parse_v3f(&tri.v0, tokens[1]) || !parse_v3f(&tri.v1, tokens[2])
		|| !parse_v3f(&tri.v2, tokens[3]) || !parse_col(&color, tokens[4]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (obj == NULL)
		return (false);
	obj->t.up = g_v3f_up;
	obj->t.pos = get_mid_point(tri.v0, tri.v1, tri.v2);
	obj->t.dir = get_normal(tri.v0, tri.v1, tri.v2);
	obj->r.color = color;
	if (!assign_material(obj, &scene->shared_materials, tokens[5]))
		return (free(obj), false);
	obj->tri = tri;
	obj->intersect = triangle_intersect;
	obj->calc_norm = triangle_normal;
	obj->type = OBJ_TRIANGLE;
	if (!vector_add(&scene->objects, obj))
		return (false);
	return (true);
}
