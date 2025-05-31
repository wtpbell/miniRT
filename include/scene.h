/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:55:45 by jboon             #+#    #+#             */
/*   Updated: 2025/05/31 12:57:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

# include "MLX42/MLX42.h"
# include "matrix.h"
# include "vector.h"
# include "color.h"
# include "libft.h"
# include "container.h"
# include <stdio.h>

typedef struct s_object	t_obj;
typedef struct s_ray	t_ray;
typedef int				(*t_intsct)(t_obj *obj, t_ray *ray, t_v2f t, float *dst);
typedef t_v3f			(*t_cnorm)(t_obj *obj, t_v3f point);

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
	t_col32		bg_col;
	mlx_image_t	*img_plane;
	t_mat4x4	view_matrix;
}	t_cam;

typedef enum e_material_type
{
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
}	t_mat_type;

typedef struct s_lambertian
{
	float	specular;
	float	shininess;
}	t_lamb;

typedef struct s_metal
{
	float	fuzz;
}	t_metal;

typedef struct s_dielectric
{
	float	ir;
	float	transmittance;
}	t_diel;

typedef struct s_material
{
	t_mat_type	type;
	t_v3f		albedo;
	union
	{
		t_lamb	lamb;
		t_metal	metal;
		t_diel	diel;
	};
}	t_mat;

typedef struct s_render
{
	t_mat	*mat;
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

typedef struct s_lighting
{
	t_v3f	light_dir;
	t_v3f	view_dir;
	t_v3f	hit_point;
	t_v3f	normal;
	t_col32	light_color;
	t_col32	obj_color;
	float	intensity;
	float	specular;
	float	diffuse;
	float	distance;
}	t_lighting;

typedef struct s_ray
{
	t_v3f	origin;
	t_v3f	direction;
}	t_ray;

typedef struct s_ray_hit
{
	t_v3f	hit;
	t_v3f	normal;
	float	distance;
	bool	front_face;
	t_obj	*obj;
	t_ray	*ray;
}	t_ray_hit;

struct s_object
{
	t_trans		t;
	t_ren		r;
	union
	{
		t_sp	sp;
		t_pl	pl;
		t_cy	cy;
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
