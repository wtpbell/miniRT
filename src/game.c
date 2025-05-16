/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/16 15:30:13 by jboon         ########   odam.nl         */
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

// https://stackoverflow.com/questions/18558910/direction-vector-to-rotation-matrix (column major, right-handed)
void	cam_to_world_mat(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	trans_m4x4(mat, pos);
	x_axis = v3f_norm(v3f_cross(up, dir));
	y_axis = v3f_norm(v3f_cross(dir, x_axis));
	mat[0] = -x_axis.x;
	mat[1] = x_axis.y;
	mat[2] = x_axis.z;
	mat[4] = y_axis.x;
	mat[5] = y_axis.y;
	mat[6] = y_axis.z;
	mat[8] = dir.x;
	mat[9] = dir.y;
	mat[10] = dir.z;
}

static bool	cam_init(t_camera *cam, mlx_t *mlx)
{
	cam->img_plane = mlx_new_image(mlx, mlx->width, mlx->height);
	if (cam->img_plane == NULL
		|| mlx_image_to_window(mlx, cam->img_plane, 0, 0) == -1)
		return (false);
	cam->aspect_ratio = cam->img_plane->width / (float)cam->img_plane->height;
	cam->bg_col = init_col32(127, 0, 127, 255);
	cam->t.dir = v3f_norm(cam->t.dir);
	cam_to_world_mat(cam->cam_to_world, cam->t.pos, cam->t.dir, init_v3f(0, 1, 0));
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
