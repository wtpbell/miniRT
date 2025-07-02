/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matrix_space.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/02 18:15:06 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/02 18:15:28 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include "matrix.h"

void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f		x_axis;
	t_v3f		y_axis;
	t_v3f		z_axis;
	t_mat4x4	rot;
	t_mat4x4	trans;

	y_axis = v3f_norm(dir);
	if (fabs(v3f_dot(y_axis, up)) > .99f)
		up = g_v3f_forward;
	x_axis = v3f_norm(v3f_cross(up, y_axis));
	z_axis = v3f_cross(x_axis, y_axis);
	id_m4x4(trans);
	rotate_m4x4(rot, x_axis, y_axis, z_axis);
	trans_m4x4(trans, pos);
	mul_mat4x4(dst, trans, rot);
}

void	view_matrix(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	if (fabs(v3f_dot(dir, up)) > .99f)
		up = g_v3f_forward;
	x_axis = v3f_norm(v3f_cross(dir, up));
	y_axis = v3f_cross(x_axis, dir);
	mat[0] = x_axis.x;
	mat[1] = x_axis.y;
	mat[2] = x_axis.z;
	mat[4] = y_axis.x;
	mat[5] = y_axis.y;
	mat[6] = y_axis.z;
	mat[8] = -dir.x;
	mat[9] = -dir.y;
	mat[10] = -dir.z;
	mat[12] = -v3f_dot(x_axis, pos);
	mat[13] = -v3f_dot(y_axis, pos);
	mat[14] = v3f_dot(dir, pos);
}
