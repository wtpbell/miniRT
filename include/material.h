/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/15 12:06:46 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

# include "MLX42/MLX42.h"
# include "rt_types.h"
# include "container.h"

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
	TEX_SOLID	= 0x0,
	TEX_CHECKER	= 0x1,
	TEX_IMAGE	= 0x2,
	TEX_PERLIN	= 0x4,
	TEX_PINK	= 0x8,
	TEX_WOOD	= 0x10,
	TEX_TURB	= 0x20,
	TEX_MARB	= 0x40,
	TEX_IS_PERLIN = TEX_PERLIN | TEX_PINK | TEX_WOOD | TEX_TURB | TEX_MARB
}	t_tex_type;

struct s_perlin
{
	float		rate;	// Change in frequency
	float		gain;	// Change in amplitude
	float		freq;	// Start frequency
	float		ampt;	// Start amplitude
	int			layers;	// Amount of layers
	struct s_marble
	{
		float	distortion;
		float	scale;
	}			marble;
};

struct s_texture
{
	t_tex_type		type;
	t_v3f			col;
	union
	{
		t_v3f	scale_rot;
		float	scale;
	};
	char			*tex_path;
	mlx_texture_t	*tex;
	t_fp_perlin		fp_perlin;
	t_perlin		p_data;
};

struct s_bump_context
{
	t_v3f					n;			// Surface normal
	t_v3f					t;			// Tangent
	t_v3f					b;			// Bitangent
	t_v2f					delta;		// Delta for sampling
	float					size;		// delta magnitude
	float					scale;		// Bump scale
	union
	{
		t_fp_perlin			fp_perlin;	// Perlin noise pattern
		const mlx_texture_t	*tex;		// Bump map texture
	};
	t_tex_type				type;		// Type of bump map (Texture or Perlin)
	const t_perlin			*p_data;
};

struct s_material
{
	char			*name;
	t_mat_type		type;
	t_v3f			albedo;
	t_tex			texture;
	t_tex			bump_map;
	t_texcol		get_texcol;
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
t_v2f	cone_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v2f	plane_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v2f	sphere_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v2f	triangle_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v2f	cylinder_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v2f	mesh_texcoord(t_obj *obj, t_v3f point, t_result *res);
t_v3f	checker_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	solid_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	image_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);
t_v3f	perturb_normal(const t_mat *mat, const t_v2f texcoord,
			const t_v3f normal);
t_v3f	sample_texture(const mlx_texture_t *tex, const t_v2f uv,
			const t_v3f mod);
t_v3f	display_normal(t_ray_hit *hit_info);
t_v2f	tri_uv_texcoord(t_tri *tri, const t_v2f weight);
t_v3f	noise_pattern(const t_v2f *texcoord, const t_tex *tex, t_v3f col_a);

float	pink_noise(t_v2f point, const t_perlin *pink);
float	turbulence_noise(t_v2f point, const t_perlin *turb);
float	marble_noise(t_v2f point, const t_perlin *marb);
float	wood_noise(t_v2f point, const t_perlin *wood);

#endif
