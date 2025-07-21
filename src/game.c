/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/07/22 01:09:18 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include <stdio.h>	

#define WIDTH    1600
#define HEIGHT   900

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(game->mlx);
	else if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		game->ui.show_ui = !game->ui.show_ui;
		render(game->scene);
		if (game->ui.show_ui)
			draw_ui(game);
	}
}

void	mouse_hook(mouse_key_t button, action_t action, 
	__attribute__((unused)) modifier_key_t mods, void *param)
{
	t_game	*game;
	
	game = param;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && game->ui.show_ui)
	{
		int32_t x, y;
		mlx_get_mouse_pos(game->mlx, &x, &y);
	}
}

bool	init_ui(t_game *game, t_scene *scene)
{
	game->ui.ui_layer = mlx_new_image(game->mlx, 300, HEIGHT);
	if (!game->ui.ui_layer)
		return false;
	game->ui.show_ui = true;
	ui_init(&game->ui, scene);
	if (mlx_image_to_window(game->mlx, game->ui.ui_layer, 0, 0) == -1)
	{
		mlx_delete_image(game->mlx, game->ui.ui_layer);
		return false;
	}
	
	return true;
}

bool	cam_init(t_cam *cam, mlx_t *mlx)
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

void	cleanup_mlx(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		if (game->scene && game->scene->camera.img_plane)
			mlx_delete_image(game->mlx, game->scene->camera.img_plane);
		if (game->ui.ui_layer)
			mlx_delete_image(game->mlx, game->ui.ui_layer);
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}

int	game(t_scene *scene)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (1);
	if (!cam_init(&scene->camera, game.mlx))
		return (cleanup_mlx(&game), 1);
	if (!init_ui(&game, scene))
	{
		printf("Warning: Failed to initialize UI\n");
		game.ui.show_ui = false;
	}
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	render(scene);
	if (game.ui.show_ui)
		draw_ui(&game);
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (0);
}
