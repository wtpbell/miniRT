/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 17:04:12 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "rt_thread.h"
#include "rt_math.h"
#include "game.h"

static inline bool	is_key_down(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key
		&& (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT));
}

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

static void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (is_key_down(keydata, MLX_KEY_H) && game->ui)
			toggle_ui_visibility(game->ui);
	// TODO: quit.c is not being used
	if (is_key_down(keydata, MLX_KEY_ESCAPE))
		game->state = GS_QUIT;
}

static void	mouse_hook(mouse_key_t button, action_t action,
	__attribute__((unused)) modifier_key_t mods, void *param)
{
	t_game	*game;
	int32_t	x;
	int32_t	y;

	game = (t_game *)param;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		if (game->ui && game->ui->context && game->ui->context->is_visible)
		{
			mlx_get_mouse_pos(game->mlx, &x, &y);
			handle_ui_click(game->ui->root, x, y, game->ui->context);
		}
	}
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

static void	set_game_state(t_game *game, t_game_state state)
{
	game->state = state;
	game->img->enabled = false;
	game->load_screen->bg.img->enabled = false;
	game->ui->context->canvas->enabled = false;
}

int	game(t_scene *scene, t_sample *sample)
{
	t_game			game;

	ft_bzero(&game, sizeof(t_game));
	// game.state = GS_RENDER; // TODO: Setting state should set certain settings

	// GS_RENDER
	// set render image to disabled
	// set ui image to disabled
	// set load image to disabled
	// set start state to GS_RENDER

	game.scene = scene;
	game.sample = sample;

	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (cleanup_mlx(&game), 1);

	if (!cam_init(&scene->camera, game.mlx))
		return (cleanup_mlx(&game), 1);
	game.img = scene->camera.img_plane;

	game.ui = create_ui(game.mlx, scene, game.sample, &game);
	if (!game.ui)
		return (cleanup_mlx(&game), 1);

	game.load_screen = init_load_screen(game.mlx);
	if (game.load_screen == NULL)
		return (cleanup_mlx(&game), 1);
	game.load_screen->bg.img->enabled = true;

	game.thread_data = init_thread_data(THRD_CNT, &game);
	if (game.thread_data == NULL)
		return (cleanup_mlx(&game), 1);

	mlx_set_instance_depth(&game.img->instances[0], 0);
	mlx_set_instance_depth(&game.load_screen->bg.img->instances[0], 1);

	set_game_state(&game, GS_RENDER);

	mlx_loop_hook(game.mlx, render_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	mlx_loop(game.mlx);
	printf("Clean up\n");
	cleanup_mlx(&game);
	return (EXIT_SUCCESS);
}
