#include "scene.h"

void	init_material(t_mat *mat, t_mat_type type)
{
	mat->type = type;
	mat->albedo = (t_v3f){.x = 1.0f, .y = 1.0f, .z = 1.0f};

	if (type == MAT_LAMBERTIAN)
	{
		mat->data.lambertian.specular = 0.7f;
		mat->data.lambertian.shininess = 64.0f;
	}
	else if (type == MAT_METAL)
		mat->data.metal.fuzz = 0.1f;
	else if (type == MAT_DIELECTRIC)
	{
		mat->data.dielectric.ir = 1.5f;
		mat->data.dielectric.transmittance = 0.9f;
	}
}

t_mat	create_lambertian(t_v3f albedo, float specular, float shininess)
{
	t_mat	mat;

	init_material(&mat, MAT_LAMBERTIAN);
	mat.albedo = albedo;
	mat.data.lambertian.specular = specular;
	mat.data.lambertian.shininess = shininess;
	return (mat);
}

t_mat	create_metal(t_v3f albedo, float fuzz)
{
	t_mat	mat;

	init_material(&mat, MAT_METAL);
	mat.albedo = albedo;
	mat.data.metal.fuzz = fuzz;
	return (mat);
}

t_mat	create_dielectric(t_v3f albedo, float ir, float transmittance)
{
	t_mat	mat;

	init_material(&mat, MAT_DIELECTRIC);
	mat.albedo = albedo;
	mat.data.dielectric.ir = ir;
	mat.data.dielectric.transmittance = transmittance;
	return (mat);
}
