/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   material.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/29 13:47:23 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/29 14:25:07 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef MATERIAL_H
# define MATERIAL_H

# include "scene.h"

t_mat	*init_material(t_mat_type type);
t_mat	*create_lambertian(t_v3f albedo, float specular, float shininess);
t_mat	*create_metal(t_v3f albedo, float fuzz);
t_mat	*create_dielectric(t_v3f albedo, float ir, float transmittance);

#endif
