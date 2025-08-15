/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_types.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 16:31:03 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 14:20:36 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*******************************************************************************
DO NOT INCLUDE THIS HEADER IN YOUR SOURCE FILES!
Use the proper header files if you need access to any of these type definitions.
*******************************************************************************/
#ifndef RT_TYPES_H
# define RT_TYPES_H

# include "vector.h"

/* light */

typedef struct s_lighting		t_lighting;

/* materials */
typedef enum e_material_type	t_mat_type;
typedef struct s_material		t_mat;
typedef struct s_texture		t_tex;
typedef struct s_bump			t_bump;
typedef struct s_bump_context	t_bump_ctx;
typedef struct s_perlin			t_perlin;

/* scene */
typedef enum e_object_type		t_obj_type;
typedef enum e_light_type		t_light_type;
typedef enum e_scene_flags		t_scene_flags;
typedef struct s_transform		t_trans;
typedef struct s_camera			t_cam;
typedef struct s_render			t_ren;
typedef struct s_plane			t_pl;
typedef struct s_sphere			t_sp;
typedef struct s_cylinder		t_cy;
typedef struct s_triangle		t_tri;
typedef struct s_cone			t_cone;
typedef struct s_light			t_light;
typedef struct s_ray			t_ray;
typedef struct s_ray_hit		t_ray_hit;
typedef struct s_object			t_obj;
typedef struct s_tri_var		t_tri_var;
typedef struct s_scene			t_scene;
typedef struct s_game			t_game;
typedef struct s_sample			t_sample;

/* ui */
typedef enum e_ui_type			t_ui_type;
typedef struct s_ui				t_ui;
typedef struct s_ui_context		t_ui_context;
typedef struct s_ui_element		t_ui_element;

typedef int						(*t_intsct)(t_obj *obj, t_ray *ray,
									t_v2f t, t_v3f *s);
typedef t_v3f					(*t_cnorm)(t_obj *obj, t_v3f point);
typedef t_v3f					(*t_apply_light)(t_scene *scene,
										t_ray_hit *hit_info, t_light *light);
typedef t_v2f					(*t_texcoord)(t_obj *obj, t_v3f point,
									t_v2f *weight);
typedef t_v3f					(*t_texcol)(const t_v2f *texcoord,
											const t_tex *tex, t_v3f prim_col);
typedef float					(*t_fp_perlin)(t_v2f uv, const t_perlin *data);

#endif
