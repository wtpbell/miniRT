/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:55:45 by jboon             #+#    #+#             */
/*   Updated: 2025/05/14 10:01:40 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "vector.h"
# include "color.h"
# include "libft.h"
# include "get_next_line.h"

typedef enum e_light_type
{
	LIGHT_AMBIENT,
	LIGHT_SPOT,
}	t_light_type;

typedef struct s_object
{
	void	*obj;
	void	(*ray_intersect)(void *obj, void *ctx);
}	t_object;

typedef struct s_transform
{
	t_v3f	pos;
	t_v3f	dir;
}	t_transform;

typedef struct s_camera
{
	t_transform	t;
	float		fov;
}	t_camera;

typedef struct s_material
{
}	t_material;

typedef struct s_render
{
	t_material	mat;
	t_col32		col;
}	t_render;

typedef struct s_plane
{
	t_transform	t;
	t_render	r;
}	t_plane;

typedef struct s_sphere
{
	t_transform	t;
	t_render	r;
	float		radius;
}	t_sphere;

typedef struct s_cylinder
{
	t_transform	t;
	t_render	r;
	float		radius;
	float		height;
}	t_cylinder;

typedef struct s_light
{
	t_v3f			pos;
	t_col32			col;
	t_light_type	type;
	float			intensity;
}	t_light;

#endif
