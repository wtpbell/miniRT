/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:55:45 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/02 18:36:09 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "MLX42/MLX42.h"
# include "matrix.h"
# include "vector.h"
# include "material.h"
# include "color.h"
# include "libft.h"
# include "container.h"
# include "ray.h"

typedef struct s_object	t_obj;
// typedef struct s_ray	t_ray;
typedef int				(*t_intsct)(t_obj *obj, t_ray *ray,
							t_v2f t, float *dst);
typedef t_v3f			(*t_cnorm)(t_obj *obj, t_v3f point);

typedef enum e_object_type
{
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_PLANE,
	OBJ_TRIANGLE
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
	t_v3f		pos;
	t_v3f		dir;
	t_v3f		up;
	t_mat4x4	to_world;
	t_mat4x4	to_obj;
}	t_trans;

typedef struct s_camera
{
	t_trans		t;
	float		fov;
	float		aspect_ratio;
	t_v3f		bg_color;
	mlx_image_t	*img_plane;
	t_mat4x4	view_matrix;
}	t_cam;

typedef struct s_render
{
	t_mat	*mat;
	t_v3f	color;
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

typedef struct s_triangle
{
	t_v3f	v0;
	t_v3f	v1;
	t_v3f	v2;
}	t_tri;

typedef struct s_light
{
	t_v3f			pos;
	t_v3f			color;
	t_light_type	type;
	float			intensity;
}	t_light;

struct s_object
{
	t_trans		t;
	t_ren		r;
	union
	{
		t_sp	sp;
		t_pl	pl;
		t_cy	cy;
		t_tri	tri;
	} ;
	t_obj_type	type;
	t_intsct	intersect;
	t_cnorm		calc_norm;
};

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_cam		camera;
	int			scene_flags;
	uint32_t	frame_num;
}	t_scene;

#endif
