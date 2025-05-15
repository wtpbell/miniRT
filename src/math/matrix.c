/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matrix.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 08:55:34 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/15 11:19:50 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"
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

t_v3f	mul_v3_m4x4(t_v3f v, t_mat4x4 mat)
{
	t_v3f	t;
	float	w;

	t.x = v.x * mat[0] + v.y * mat[4] + v.z * mat[8] + mat[12];
	t.y = v.x * mat[1] + v.y * mat[5] + v.z * mat[9] + mat[13];
	t.z = v.x * mat[2] + v.y * mat[6] + v.z * mat[10] + mat[14];
	w = v.x * mat[3] + v.y * mat[7] + v.z * mat[11] + mat[15];
	if (w != 1.0f && w != 0.0f)
	{
		w = 1.0f / w;
		t.x *= w;
		t.y *= w;
		t.z *= w;
	}
	return (t);
}

t_v3f	mul_dir_m4x4(t_v3f dir, t_mat4x4 mat)
{
	t_v3f	t;

	t.x = dir.x * mat[0] + dir.y * mat[4] + dir.z * mat[8];
	t.y = dir.x * mat[1] + dir.y * mat[5] + dir.z * mat[9];
	t.z = dir.x * mat[2] + dir.y * mat[6] + dir.z * mat[10];
	return (t);
}

void	mul_mat4x4(t_mat4x4 dst, t_mat4x4 a, t_mat4x4 b)
{
	int	i;
	int	r;
	int	c;

	i = 0;
	while (i < M4X4_SIZE)
	{
		r = (i >> 2);
		r <<= 2;
		c = i - r;
		dst[i] = a[r] * b[c]
			+ a[r + 1] * b[c + 4]
			+ a[r + 2] * b[c + 8]
			+ a[r + 3] * b[c + 12];
		++i;
	}
}
