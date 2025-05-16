/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:55:45 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 11:08:17 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "MLX42/MLX42.h"
# include "vector.h"
# include "color.h"
# include "libft.h"
# include "get_next_line.h"
# include "container.h"

typedef enum e_light_type
{
	LIGHT_AMBIENT,
	LIGHT_POINT,
}	t_light_type;

typedef enum e_scene_flags
{
	SCENE_NONE = 0,
	SCENE_AMBIENT = 1 << 0,
	SCENE_POINT_LIGHT = 1 << 1,
	SCENE_CAMERA = 2 << 2
}	t_scene_flags;

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
	mlx_image_t	*img_plane;
	float		aspect_ratio;
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

typedef struct s_scene
{
	t_vector		objects;
	t_vector		lights;
	t_camera		camera;
	int				scene_flags;
}	t_scene;

#endif
