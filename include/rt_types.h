/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_types.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 16:31:03 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/27 20:56:36 by jboon         ########   odam.nl         */
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

typedef int						(*t_intsct)(t_obj *obj, t_ray *ray,
									t_v2f t, float *dst);
typedef t_v3f					(*t_cnorm)(t_obj *obj, t_v3f point);
typedef t_v3f					(*t_apply_light)(t_scene *scene,
										t_ray_hit *hit_info, t_light *light);
typedef t_v2f					(*t_texcoord)(t_obj *obj, t_v3f point);
typedef t_v3f					(*t_texcol)(const t_v2f *texcoord,
											const t_tex *tex, t_v3f prim_col);

#endif
