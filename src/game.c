/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/08/16 21:39:41 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "rt_thread.h"
#include "rt_math.h"
#include "game.h"
#include "perlin_display.h"

void	cleanup_mlx(t_game *game)
{
	if (game->ui)
	{
		destroy_ui(game->ui);
		game->ui = NULL;
	}
	if (game->mlx)
	{
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game		*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
	{
		if (game->ui)
			toggle_ui_visibility(game->ui);
	}
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(game->mlx);
}

void	mouse_hook(mouse_key_t button, action_t action,
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

int	game(t_scene *scene, t_sample *sample)
{
	t_game	game;

	init_perlin();
	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (cleanup_mlx(&game), 1);
	if (!cam_init(&scene->camera, game.mlx))
		return (cleanup_mlx(&game), 1);
	game.img = scene->camera.img_plane;
	game.scene = scene;
	game.needs_redraw = true;
	game.sample = sample;
	game.ui = create_ui(game.mlx, scene, game.sample, &game);
	if (!game.ui)
		return (cleanup_mlx(&game), 1);
	mlx_set_instance_depth(&game.img->instances[0], 0);
	mlx_loop_hook(game.mlx, render_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	game.needs_redraw = true;
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (EXIT_SUCCESS);
}
