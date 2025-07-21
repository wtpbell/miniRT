/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_cylinder_uv.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/02 18:34:31 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/02 18:35:02 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"

/*
	Cylindrical coordinates (rho, phi, z)
	rho is the distance from the z-axis to the point (radius)
	phi is the angle starting from the x-axis on the xy-plane.
	z is the distance (in a vertical direction) from the xy-plane to point p
	(height)

	rho^2 = x^2 + y^2
	tan phi = y/x
	z = z

	By convention the z-axis is consider the up axis, but in our case it would
	be the y-axis
*/
t_v2f	cylinder_texcoord(t_obj *obj, t_v3f point)
{
	t_v3f	local_point;
	float	theta;
	float	y;

	local_point = mul_v3_m4x4(point, obj->t.to_obj);
	theta = atan2f(local_point.z, local_point.x) / TAU;
	y = (local_point.y / obj->cy.height);
	return (init_v2f(1.0f - (theta + 0.5f), ft_clampf01(y + 0.5f)));
}
