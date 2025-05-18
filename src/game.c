/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/05/18 21:25:46 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "debug/rt_debug.h"

#define WIDTH	1024
#define HEIGHT	1024

// TODO: Implementation needed ()
static void	update(void *ctx)
{
	(void)ctx;
}

static float	v3f_len(t_v3f v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

// http://bentonian.com/Lectures/AdvGraph1314/2.%20Ray%20tracing%20-%20all%20the%20maths.pdf
//https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices/using-4x4-matrices-transform-objects-3D.html

void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir)
{
	t_v3f		x_axis;
	t_v3f		y_axis;
	t_v3f		z_axis;
	t_mat4x4	rot;
	t_mat4x4	trans;
	
	y_axis = v3f_norm(dir); //dir to up
	z_axis = v3f_cross((t_v3f){.x = 0, .y = 1, .z = 0}, y_axis);
	if (v3f_len(z_axis) < 1e-4f)
	{
		if (y_axis.y > 0)
			id_m4x4(rot);
		else
		{
			id_m4x4(rot);
			rot[0] = -1;
			rot[10] = -1;
		}
	}
	else
	{
		z_axis = v3f_norm(z_axis);
		x_axis = v3f_cross(y_axis, z_axis);
		rotate_m4x4(rot, x_axis, y_axis, z_axis);
	}
	id_m4x4(trans);
	trans_m4x4(trans, pos);
	mul_mat4x4(dst, trans, rot);
}


void	cam_to_world(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	trans_m4x4(mat, pos);
	x_axis = v3f_norm(v3f_cross(up, dir));
	y_axis = v3f_norm(v3f_cross(dir, x_axis));
	x_axis = v3f_scale(x_axis, -1);
	mat[0] = x_axis.x;
	mat[1] = x_axis.y;
	mat[2] = x_axis.z;
	mat[4] = y_axis.x;
	mat[5] = y_axis.y;
	mat[6] = y_axis.z;
	mat[8] = dir.x;
	mat[9] = dir.y;
	mat[10] = dir.z;
}

static bool	cam_init(t_cam *cam, mlx_t *mlx)
{
	cam->img_plane = mlx_new_image(mlx, mlx->width, mlx->height);
	if (cam->img_plane == NULL
		|| mlx_image_to_window(mlx, cam->img_plane, 0, 0) == -1)
		return (false);
	cam->aspect_ratio = cam->img_plane->width / (float)cam->img_plane->height;
	cam->bg_col = init_col32(127, 0, 127, 255);
	cam->t.dir = v3f_norm(cam->t.dir);
	cam_to_world(cam->cam_to_world, cam->t.pos, cam->t.dir, init_v3f(0, 1, 0));
	return (true);
}

int	game(t_scene *scene)
{
	mlx_t	*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (cam_init(&scene->camera, mlx))
	{
		scene_print(scene);
		mlx_loop_hook(mlx, update, mlx);
		render(scene);
		mlx_loop(mlx);
	}
	mlx_terminate(mlx);
	return (0);
}
