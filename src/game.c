/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 15:36:57 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <stdio.h>
#include "MLX42/MLX42.h"
#include "minirt.h"
#include "rt_thread.h"
#include "rt_math.h"

#define WIDTH		1600
#define HEIGHT		900

static bool	cam_init(t_cam *cam, mlx_t *mlx)
{
	cam->img_plane = mlx_new_image(mlx, mlx->width, mlx->height);
	if (cam->img_plane == NULL
		|| mlx_image_to_window(mlx, cam->img_plane, 0, 0) == -1)
		return (false);
	cam->aspect_ratio = cam->img_plane->width / (float)cam->img_plane->height;
	cam->bg_color = init_v3f(0.5f, 0.0f, 0.5f);
	cam->t.dir = v3f_norm(cam->t.dir);
	update_camera_view(cam);
	return (true);
}

static void	cleanup_mlx(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}

int	game(t_scene *scene)
{
	t_game	game;

	init_perlin();
	ft_bzero(&game, sizeof(t_game));
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (1);
	if (!cam_init(&scene->camera, game.mlx))
		return (cleanup_mlx(&game), 1);
	mlx_key_hook(game.mlx, quit_on_escape, &game);
	if (thread_rendering(scene))
		mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (0);
}
