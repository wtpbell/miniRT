/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:55:45 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 18:31:36 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "MLX42/MLX42.h"
# include "ray.h"
# include "matrix.h"
# include "vector.h"
# include "color.h"
# include "libft.h"
# include "container.h"

typedef struct s_object t_obj;

typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_PLANE,
}	t_obj_type;

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

typedef struct s_transform
{
	t_v3f	pos;
	t_v3f	dir;
}	t_trans;

typedef struct s_camera
{
	t_trans		t;
	float		fov;
	float		aspect_ratio;
	t_col32		bg_col;
	mlx_image_t	*img_plane;
	t_mat4x4	cam_to_world;
}	t_cam;

typedef struct s_material
{
}	t_mat;

typedef struct s_render
{
	t_mat	mat;
	t_col32	col;
}	t_ren;

typedef struct s_plane
{
}	t_pl;

typedef struct s_sphere
{
	float		radius;
}	t_sp;

typedef struct s_cylinder
{
	float		radius;
	float		height;
}	t_cy;

typedef struct s_light
{
	t_v3f			pos;
	t_col32			col;
	t_light_type	type;
	float			intensity;
}	t_light;

struct s_object
{
	t_trans	t;
	t_ren	r;
	union
	{
		t_sp	sp;
		t_pl	pl;
		t_cy	cy;
	}			shape;
	t_obj_type	type;
	bool		(*intersect)(t_obj *obj, t_ray *ray, float *dst);
};

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_cam		camera;
	int			scene_flags;
}	t_scene;

#endif
