/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/23 12:24:40 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "debug/rt_debug.h"

#define WIDTH	1024
#define HEIGHT	1024

static void	update(void *ctx)
{
	(void)ctx;
}

// TODO: Implementation needed ()
void	obj_to_world(t_mat4x4 dst, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f		x_axis;
	t_v3f		y_axis;
	t_v3f		z_axis;
	t_mat4x4	rot;
	t_mat4x4	trans;

	y_axis = v3f_norm(dir);
	if (fabs(v3f_dot(y_axis, up)) > .99f)
		up = (t_v3f){.x = 0, .y = 0, .z = 1};
	x_axis = v3f_norm(v3f_cross(up, y_axis));
	z_axis = v3f_cross(x_axis, y_axis);
	id_m4x4(trans);
	trans_m4x4(trans, pos);
	rotate_m4x4(rot, x_axis, y_axis, z_axis);
	mul_col_mat4x4(dst, trans, rot);
}

void	view_matrix(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	if (fabs(v3f_dot(dir, up)) > .99f)
		up = (t_v3f){.x = 0, .y = 0, .z = 1};
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

static bool	cam_init(t_cam *cam, mlx_t *mlx)
{
	cam->img_plane = mlx_new_image(mlx, mlx->width, mlx->height);
	if (cam->img_plane == NULL
		|| mlx_image_to_window(mlx, cam->img_plane, 0, 0) == -1)
		return (false);
	cam->aspect_ratio = cam->img_plane->width / (float)cam->img_plane->height;
	cam->bg_col = init_col32(127, 0, 127, 255);
	cam->t.dir = v3f_norm(cam->t.dir);
	view_matrix(cam->view_matrix, cam->t.pos, cam->t.dir, cam->t.up);
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
