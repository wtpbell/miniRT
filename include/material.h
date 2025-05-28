#ifndef MATERIAL_H
# define MATERIAL_H

# include "scene.h"

void	init_material(t_mat *mat, t_mat_type type);
t_mat	create_lambertian(t_v3f albedo, float specular, float shininess);
t_mat	create_metal(t_v3f albedo, float fuzz);
t_mat	create_dielectric(t_v3f albedo, float ir, float transmittance);

#endif
