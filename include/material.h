/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/04 14:56:09 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "MLX42/MLX42.h"
# include "rt_types.h"
# include "container.h"

typedef struct s_material	t_mat;

enum e_material_type
{
	MAT_UNKNOWN,
	MAT_LAMBERTIAN,
	MAT_METAL,
	MAT_DIELECTRIC,
	MAT_NORMAL
};

typedef enum e_texture_type
{
	TEX_SOLID,
	TEX_CHECKER,
	TEX_IMAGE
}	t_tex_type;

struct s_texture
{
	t_tex_type		type;
	t_v3f			scale_rot;
	t_v3f			col;
	mlx_texture_t	*tex;
};

typedef struct s_bump_context
{
	t_v3f		n;			// Surface normal
	t_v3f		t;			// Tangent
	t_v3f		b;			// Bitangent
	t_v3f		heights[3];	// Height samples (center, u+delta, v+delta)
	t_v3f		p;			// Perturbed normal
	float		delta;		// Delta for sampling
	const t_mat	*mat;		// Material (contains bump_scale)
}	t_bump;

struct s_material
{
	char			*name;
	t_mat_type		type;
	t_v3f			albedo;
	t_tex			texture;
	t_texcol		get_texcol;
	char			*bump_path;
	char			*tex_path;
	mlx_texture_t	*bump_map;
	float			bump_scale;
	bool			debug_bump; // Toggle bump map visualization
	struct s_lambertian
	{
		float		specular;
		float		shininess;
		float		roughness;
	}				lamb;
	struct s_metal
	{
		float		roughness;
	}				metal;
	struct s_dielectric
	{
		float		ir;
		float		transmittance;
		float		roughness;
	}				diel;
};

t_mat	*init_material(t_mat_type type, const char *name);
bool	create_default_materials(t_vector *shared_materials);
bool	assign_material(t_obj *obj, t_vector *materials, const char *m_name);
t_v3f	blend_color(t_scene *sc, t_ray_hit *h, uint32_t depth, float ior);
t_v3f	handle_dielectric(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v3f	handle_lambertian(t_scene *scene, t_ray_hit *hit_info);
t_v3f	handle_metal(t_scene *sc, t_ray_hit *hit, uint32_t depth);
t_v2f	cone_texcoord(t_obj *obj, t_v3f point, t_v2f *weight);
t_v2f	plane_texcoord(t_obj *obj, t_v3f point, t_v2f *weight);
t_v2f	sphere_texcoord(t_obj *obj, t_v3f point, t_v2f *weight);
t_v2f	triangle_texcoord(t_obj *obj, t_v3f point, t_v2f *weight);
t_v2f	cylinder_texcoord(t_obj *obj, t_v3f point, t_v2f *weight);
t_v2f	mesh_texcoord(t_obj *obj, t_v3f point);
t_v3f	checker_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	solid_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	image_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	perturb_normal(const t_mat *mat, const t_v2f texcoord,
			const t_v3f normal);
t_v3f	sample_texture(const mlx_texture_t *tex, const t_v2f uv,
			const t_v3f mod);
t_v3f	display_normal(t_ray_hit *hit_info);

#endif
