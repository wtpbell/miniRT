/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/18 16:57:19 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "rt_types.h"
# include "container.h"

enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC
};

typedef enum e_texture_type
{
	TEX_SOLID,
	TEX_CHECKER,
}	t_tex_type;

struct s_texture
{
	t_tex_type	type;
	t_v3f		scale_rot;
	t_v3f		col;
};

struct s_material
{
	char		*name;
	t_mat_type	type;
	t_v3f		albedo;
	t_tex		texture;
	t_texcol	get_texcol;
	struct {
		float	specular;
		float	shininess;
		float	roughness;
	}	lamb;
	struct {
		float	roughness;
	}	metal;
	struct {
		float	ir;
		float	transmittance;
		float	roughness;
	}	diel;
};

t_mat	*init_material(t_mat_type type, const char *name);
bool	create_default_materials(t_vector *shared_materials);
bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name);
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info);
t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth);

t_v2f	plane_texcoord(t_obj *obj, t_v3f point);
t_v2f	sphere_texcoord(t_obj *obj, t_v3f point);
t_v2f	triangle_texcoord(t_obj *obj, t_v3f world_point);
t_v2f	cylinder_texcoord(t_obj *obj, t_v3f point);
t_v3f	checker_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	solid_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);

#endif
