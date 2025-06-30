/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matrix.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 08:55:34 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/30 19:25:07 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "libft.h"

void	id_m4x4(t_mat4x4 mat)
{
	int	i;

	i = 0;
	ft_bzero(mat, sizeof(t_mat4x4));
	while (i < M4X4_SIZE)
	{
		mat[i] = 1;
		i += 5;
	}
}

void	trans_m4x4(t_mat4x4 mat, t_v3f trans)
{
	mat[12] = trans.x;
	mat[13] = trans.y;
	mat[14] = trans.z;
}

void	scale_m4x4(t_mat4x4 mat, t_v3f scale)
{
	mat[0] = scale.x;
	mat[5] = scale.y;
	mat[10] = scale.z;
}

void	rotate_m4x4(t_mat4x4 rot, t_v3f x, t_v3f y, t_v3f z)
{
	id_m4x4(rot);
	rot[0] = x.x;
	rot[4] = y.x;
	rot[8] = z.x;
	rot[1] = x.y;
	rot[5] = y.y;
	rot[9] = z.y;
	rot[2] = x.z;
	rot[6] = y.z;
	rot[10] = z.z;
}
