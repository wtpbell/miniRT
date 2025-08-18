/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 11:09:14 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "rt_thread.h"
#include "rt_math.h"
#include "game.h"

static void	cleanup_mlx(t_game *game)
{
	if (game->ui)
		destroy_ui(game->ui);
	if (game->load_screen)
		destroy_load_screen(game->load_screen, game->mlx);
	if (game->mlx)
		mlx_terminate(game->mlx);
	if (game->thread_data)
		cleanup_thread_data(game->thread_data);
	game->ui = NULL;
	game->load_screen = NULL;
	game->mlx = NULL;
	game->thread_data = NULL;
}

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

void	set_game_state(t_game *game, t_game_state state)
{
	bool *const	*ptr;
	bool *const	img_states[4] = {&game->img->enabled,
		&game->load_screen->bg.img->enabled,
		&game->ui->context->canvas->enabled, NULL
	};
	const bool	toggle_state[3][3] = {
	[GS_IDLE] = {true, false, true},
	[GS_RENDER] = {true, false, false},
	[GS_LOAD] = {false, true, false}
	};

	game->state = state;
	ptr = img_states;
	while (*ptr)
	{
		**ptr = toggle_state[state][ptr - img_states];
		++ptr;
	}
}

static bool	game_init(t_game *game, t_scene *scene, t_sample *sample)
{
	ft_bzero(game, sizeof(t_game));
	game->scene = scene;
	game->sample = sample;
	game->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game->mlx)
		return (cleanup_mlx(game), false);
	if (!cam_init(&scene->camera, game->mlx))
		return (cleanup_mlx(game), false);
	game->img = scene->camera.img_plane;
	game->ui = create_ui(game->mlx, scene, game->sample, game);
	if (!game->ui)
		return (cleanup_mlx(game), false);
	game->load_screen = init_load_screen(game->mlx);
	if (game->load_screen == NULL)
		return (cleanup_mlx(game), false);
	game->load_screen->bg.img->enabled = true;
	game->thread_data = init_thread_data(THRD_CNT, game);
	if (game->thread_data == NULL)
		return (cleanup_mlx(game), false);
	mlx_set_instance_depth(&game->img->instances[0], 0);
	mlx_set_instance_depth(&game->ui->context->canvas->instances[0], 1);
	mlx_set_instance_depth(&game->load_screen->bg.img->instances[0], 2);
	set_game_state(game, GS_RENDER);
	return (true);
}

int	game(t_scene *scene, t_sample *sample)
{
	t_game	game;

	if (!game_init(&game, scene, sample))
		return (EXIT_FAILURE);
	mlx_loop_hook(game.mlx, render_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	mlx_loop(game.mlx);
	cancel_threads(game.thread_data->threads, game.thread_data->thread_count);
	cleanup_mlx(&game);
	return (EXIT_SUCCESS);
}
