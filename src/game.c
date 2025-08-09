/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/08/09 16:06:29 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "rt_thread.h"
#include "ui.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_game t_game;

void	render_loop(void *param)
{
	t_game			*game;
	t_ui_context	*ctx;

	game = (t_game *)param;
	if (game->needs_redraw)
	{
		ft_memset(game->img->pixels, 0, game->img->width * game->img->height * sizeof(int));
		if (thread_rendering(game->scene, game->sample))
		{
			game->img->instances[0].enabled = true;
			game->needs_redraw = false;
		}
	}
	if (game->ui && game->ui->context)
	{
		ctx = game->ui->context;
		if (ctx->is_visible && ctx->canvas)
			render_ui(game->ui);
	}
}

t_ui	*create_ui(mlx_t *mlx, t_scene *scene, t_sample *sample, void *game_ptr)
{
	t_ui			*ui;
	t_ui_element	*ui_sections;
	t_v2f			size;

	ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
		return (NULL);
	ui->context = create_ui_context(mlx, scene, game_ptr);
	if (!ui->context)
		return (free(ui), NULL);
	size = init_v2f(UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(ui->context, g_v2f_zero, size);
	if (!ui->root)
		return (destroy_ui(ui), NULL);
	ui_sections = create_ui_sections(ui->context, sample, g_v2f_zero, size);
	if (!ui_sections)
		return (destroy_ui(ui), NULL);
	attach_child(ui->root, ui_sections);
	ui->context->needs_redraw = true;
	return (ui);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
	{
		if (game->ui)
			toggle_ui_visibility(game->ui);
	}
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		game->should_exit = true;
		mlx_close_window(game->mlx);
	}
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

void	cleanup_mlx(t_game *game)
{
	if (!game)
		return ;
	if (game->ui)
	{
		destroy_ui(game->ui);
		game->ui = NULL;
	}
	if (game->scene)
	{
		// Add any scene-specific cleanup here
	}
	if (game->mlx)
	{
		mlx_terminate(game->mlx);
		game->mlx = NULL;
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

	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.should_exit = false;
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
