/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matrix_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 19:24:56 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/30 19:25:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"

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

void	transpose_mat4x4(t_mat4x4 mat)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = i + 1;
		while (j < 4)
		{
			ft_swapf(mat + (i << 2) + j, mat + (j << 2) + i);
			++j;
		}
		++i;
	}
}

void	invert_m4x4(t_mat4x4 dst, const t_mat4x4 src)
{
	t_v3f	t;

	id_m4x4(dst);
	dst[0] = src[0];
	dst[1] = src[4];
	dst[2] = src[8];
	dst[4] = src[1];
	dst[5] = src[5];
	dst[6] = src[9];
	dst[8] = src[2];
	dst[9] = src[6];
	dst[10] = src[10];
	t = init_v3f(-src[12], -src[13], -src[14]);
	dst[12] = t.x * dst[0] + t.y * dst[4] + t.z * dst[8];
	dst[13] = t.x * dst[1] + t.y * dst[5] + t.z * dst[9];
	dst[14] = t.x * dst[2] + t.y * dst[6] + t.z * dst[10];
}
