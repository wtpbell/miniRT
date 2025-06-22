/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:55:45 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/22 11:38:10 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "MLX42/MLX42.h"
# include "libft.h"
# include "rt_types.h"
# include "container.h"
# include "matrix.h"
# include "vector.h"
# include "material.h"
# include "color.h"
# include "material.h"

enum e_object_type
{
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_PLANE,
	OBJ_TRIANGLE
};

enum e_light_type
{
	LIGHT_AMBIENT,
	LIGHT_POINT,
	LIGHT_SPOT,
};

typedef enum e_scene_flags
{
	SCENE_NONE = 0,
	SCENE_AMBIENT = 1 << 0,
	SCENE_POINT_LIGHT = 1 << 1,
	SCENE_CAMERA = 2 << 2
}	t_scene_flags;

struct s_transform
{
	t_v3f		pos;
	t_v3f		dir;
	t_v3f		up;
	t_mat4x4	to_world;
	t_mat4x4	to_obj;
};

struct s_camera
{
	t_trans		t;
	float		fov;
	float		aspect_ratio;
	t_v3f		bg_color;
	mlx_image_t	*img_plane;
	t_mat4x4	view_matrix;
	float		aperture;
	float		focus_dist;
	t_v3f		u;
	t_v3f		v;
	t_v3f		w;
	t_v3f		horizontal;
	t_v3f		vertical;
	t_v3f		lower_left;
};

struct s_render
{
	t_mat		*mat;
	t_v3f		color;
	t_texcoord	get_texcoord;
};

struct s_plane
{
};

struct s_sphere
{
	float		radius;
};

struct s_cylinder
{
	float		radius;
	float		height;
};

struct s_cone
{
	float		radius;
	float		height;
};

struct s_triangle
{
	t_v3f	v0;
	t_v3f	v1;
	t_v3f	v2;
	t_v3f	vt0;
	t_v3f	vt1;
	t_v3f	vt2;
	t_v2f	weight;
};

struct s_light
{
	t_v3f			pos;
	t_v3f			color;
	t_light_type	type;
	float			intensity;
	struct s_spotlight
	{
		t_v3f		dir;
		float		inner;
		float		outer;
	}				spot;
};

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
		t_cone	cone;
	};
	t_obj_type	type;
	t_intsct	intersect;
	t_cnorm		calc_norm;
};

struct s_tri_var
{
	t_v3f	v0v1;
	t_v3f	v0v2;
	t_v3f	pvec;
	float	det;
};

struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_vector	shared_materials;
	t_cam		camera;
	int			scene_flags;
	uint32_t	frame_num;
};

#endif
