/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matrix.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 08:51:07 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/20 14:27:44 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef MATRIX_H
# define MATRIX_H

# include "vector.h"

# define M4X4_SIZE	16

typedef float	t_mat4x4[16];

void	id_m4x4(t_mat4x4 mat);
void	trans_m4x4(t_mat4x4 mat, t_v3f trans);
void	scale_m4x4(t_mat4x4 mat, t_v3f scale);
t_v3f	mul_v3_m4x4(t_v3f v, t_mat4x4 mat);
t_v3f	mul_dir_m4x4(t_v3f dir, t_mat4x4 mat);
void	mul_mat4x4(t_mat4x4 dst, t_mat4x4 a, t_mat4x4 b);
void	transpose_mat4x4(t_mat4x4 mat);
void	mul_col_mat4x4(t_mat4x4 dst, t_mat4x4 a, t_mat4x4 b);
void	invert_m4x4(t_mat4x4 dst, const t_mat4x4 src);
void	rotate_m4x4(t_mat4x4 mat, t_v3f x, t_v3f y, t_v3f z);

#endif
