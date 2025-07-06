/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/07/06 20:28:13 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <stdio.h>
#include "MLX42/MLX42.h"
#include "minirt.h"
#include "rt_thread.h"

#define WIDTH		1600
#define HEIGHT		900
#define THRD_CNT	4

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

static void init_thread(t_pthread_instr *instr, t_scene *scene, uint32_t start, uint32_t end)
{
	instr->scene = scene;
	instr->start_y = start;
	instr->end_y = end;
	instr->img = scene->camera.img_plane;
	pthread_create(&instr->thread, NULL, render, instr);
}

static void	render_scene(t_scene *scene)
{
	t_pthread_instr	threads[THRD_CNT];
	uint32_t		delta;
	uint32_t		start_y;
	uint32_t		end_y;
	int				i;

	i = 0;
	start_y = 0;
	delta = scene->camera.img_plane->height / THRD_CNT;
	while (i < THRD_CNT)
	{
		end_y = start_y + delta;
		if (i == THRD_CNT - 1 || end_y > scene->camera.img_plane->height) // if last thread or end_y is past the image height
			end_y = scene->camera.img_plane->height; // clamp to the actual image height
		init_thread(&threads[i], scene, start_y, end_y);
		start_y = end_y; //  next thread starts where this thread ends
		++i;
	}
	i = 0;
	while (i < THRD_CNT)
	{
		pthread_join(threads[i].thread, NULL);
		++i;
	}
}

int	game(t_scene *scene)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (1);
	if (!cam_init(&scene->camera, game.mlx))
		return (cleanup_mlx(&game), 1);
	mlx_key_hook(game.mlx, quit_on_escape, &game);

	float starttime = (float)clock() / CLOCKS_PER_SEC;
	render_scene(scene);
	float endtime = (float)clock() / CLOCKS_PER_SEC;
	printf("TIME: %f\n", endtime - starttime);

	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (0);
}
