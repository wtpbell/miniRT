#include "scene.h"
#include "rt_math.h"

t_mat	*init_material(t_mat_type type)
{
	t_mat	*mat;

	mat = (t_mat *)ft_calloc(1, sizeof(t_mat));
	if (!mat)
		return (NULL);
	mat->type = type;
	mat->albedo = (t_v3f){.x = 1.0f, .y = 1.0f, .z = 1.0f};

	if (type == MAT_LAMBERTIAN)
	{
		mat->lamb.specular = 0.7f;
		mat->lamb.shininess = 64.0f;
	}
	else if (type == MAT_METAL)
		mat->metal.fuzz = 0.1f;
	else if (type == MAT_DIELECTRIC)
	{
		mat->diel.ir = 1.5f;
		mat->diel.transmittance = 0.9f;
	}
	return (mat);
}

t_mat	*create_lambertian(t_v3f albedo, float specular, float shininess)
{
	t_mat	*mat;

	mat = init_material(MAT_LAMBERTIAN);
	if (!mat)
		return (NULL);
	mat->albedo = albedo;
	mat->lamb.specular = specular;
	mat->lamb.shininess = shininess;
	return (mat);
}

t_mat	*create_metal(t_v3f albedo, float fuzz)
{
	t_mat	*mat;

	mat = init_material(MAT_METAL);
	if (!mat)
		return (NULL);
	mat->albedo = albedo;
	mat->metal.fuzz = fuzz;
	return (mat);
}

t_mat	*create_dielectric(t_v3f albedo, float ir, float transmittance)
{
	t_mat	*mat;

	mat = init_material(MAT_DIELECTRIC);
	if (!mat)
		return (NULL);
	mat->albedo = albedo;
	mat->diel.ir = ir;
	mat->diel.transmittance = ft_clampf(transmittance, 0.0f, 1.0f);
	return (mat);
}
