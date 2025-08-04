/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/08/04 16:57:24 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <stdio.h>
#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "rt_thread.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include "rt_thread.h"

#define WIDTH		1600
#define HEIGHT		900

typedef struct s_game t_game;

void render_loop(void *param)
{
	t_game *game = (t_game *)param;
	static bool first_render = true;
	
	if (!game || !game->img || !game->img->pixels)
		return ;

	if (first_render || game->needs_redraw)
	{
		// Clear the image
		ft_memset(game->img->pixels, 0, game->img->width * game->img->height * sizeof(int));
		
		// Render the scene
		if (thread_rendering(game->scene))
		{
			// Update the image on screen
			game->img->instances[0].enabled = true;
			game->needs_redraw = false;
			first_render = false;
		}

		// Update UI if needed
		if (game->ui && game->ui->context)
		{
			game->ui->context->needs_redraw = true;
			if (game->ui->context->is_visible)
			{
				render_ui(game->ui);
				if (game->ui->context->canvas && game->ui->context->canvas->count > 0)
				{
					mlx_set_instance_depth(&game->ui->context->canvas->instances[0], 1000);
					game->ui->context->canvas->instances[0].enabled = true;
				}
			}
		}
	}
}

t_ui *create_ui(mlx_t *mlx, t_scene *scene)
{
	t_ui		*ui;
	t_v2f		root_pos;
	t_v2f		root_size;
	t_v2f		ambient_pos;
	t_v2f		ambient_size;
	t_ui_element	*ambient_section;

	ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
		return (NULL);
	ui->context = create_ui_context(mlx, scene);
	if (!ui->context)
		return (free(ui), NULL);
	root_pos = g_v2f_zero;
	root_size = init_v2f(UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(ui->context, root_pos, root_size);
	if (!ui->root)
		return (destroy_ui(ui), NULL);
	ambient_pos = init_v2f(0, 0);
	ambient_size = init_v2f(UI_PANEL_WIDTH, 200);
	ambient_section = create_ambient_section(ui->context, scene, ambient_pos, ambient_size);
	if (!ambient_section)
		return (destroy_ui(ui), NULL);
	attach_child(ui->root, ambient_section);
	ui->context->needs_redraw = true;
	return (ui);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;

	if (keydata.key == MLX_KEY_U && keydata.action == MLX_PRESS)
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

// void mouse_hook(mouse_key_t button, action_t action, 
// 	__attribute__((unused)) modifier_key_t mods, void *param)
// {
// 	t_game	*game;
// 	int32_t	x;
// 	int32_t	y;

// 	game = (t_game *)param;
// 	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && game->ui
// 		&& game->ui->root->style.visible)
// 	{
// 		mlx_get_mouse_pos(game->mlx, &x, &y);
// 		handle_ui_click(game->ui->root, x, y, game->ui->ctx);
// 		render_ui(game->ui);
// 	}
// }

void	cleanup_mlx(t_game *game)
{
	if (!game)
		return;
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

bool	init_ui(t_game *game, t_scene *scene)
{
	fprintf(stderr, "Initializing UI...\n");
	game->ui = create_ui(game->mlx, scene);
	if (!game->ui)
	{
		fprintf(stderr, "Error: Failed to initialize UI\n");
		return (false);
	}
	fprintf(stderr, "UI initialized successfully\n");
	return (true);
}

int	game(t_scene *scene)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.should_exit = false;
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return (EXIT_FAILURE);
	if (!init_ui(&game, scene))
		fprintf(stderr, "Warning: UI initialization had issues, continuing without UI\n");
	if (!cam_init(&scene->camera, game.mlx))
	{
		fprintf(stderr, "Error: Failed to initialize camera\n");
		cleanup_mlx(&game);
		return (EXIT_FAILURE);
	}
	game.img = scene->camera.img_plane;
	if (!game.img)
	{
		fprintf(stderr, "Error: Failed to initialize game image\n");
		cleanup_mlx(&game);
		return (EXIT_FAILURE);
	}
	mlx_set_instance_depth(&game.img->instances[0], 0);
	mlx_loop_hook(game.mlx, render_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	// mlx_mouse_hook(game.mlx, mouse_hook, &game);

	fprintf(stderr, "Starting main game loop...\n");

	// Let the render_loop handle the first render
	game.needs_redraw = true;
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (EXIT_SUCCESS);
}
