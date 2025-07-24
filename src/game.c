/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/24 14:27:50 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "parser.h"
#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_game t_game;


void render_loop(void *param)
{
	t_game *game = (t_game *)param;

	if (game->ui && game->ui->needs_redraw) {
		render_ui(game->ui);
		game->ui->needs_redraw = false;
	}
}

#define WIDTH    1600
#define HEIGHT   900

t_ui	*create_ui(mlx_t *mlx, t_scene *scene)
{
	t_ui			*ui;
	t_ui_element	*ambient_section;

	printf("create_ui: Starting UI creation\n");
	if (!mlx)
	{
		printf("create_ui: Error - MLX pointer is NULL\n");
		return (NULL);
	}

	ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
	{
		printf("create_ui: Failed to allocate UI struct\n");
		return (NULL);
	}
	printf("create_ui: Allocated UI struct at %p\n", (void *)ui);
	ui->mlx = mlx;
	ui->needs_redraw = true;
	printf("create_ui: Set MLX pointer to %p\n", (void *)mlx);

	ui->ui_layer = mlx_new_image(mlx, UI_PANEL_WIDTH, HEIGHT);
	if (!ui->ui_layer)
	{
		printf("create_ui: Failed to create UI layer image\n");
		free(ui);
		return (NULL);
	}
	memset(ui->ui_layer->pixels, 0, UI_PANEL_WIDTH * HEIGHT * sizeof(int32_t));
	printf("create_ui: Created UI layer at %p, size: %dx%d\n", 
		  (void *)ui->ui_layer, UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(init_v2f(0, 0), init_v2f(UI_PANEL_WIDTH, HEIGHT));
	if (!ui->root)
	{	
		printf("create_ui: Failed to create root panel\n");
		mlx_delete_image(mlx, ui->ui_layer);
		free(ui);
		return (NULL);
	}
	printf("create_ui: Created root panel at %p\n", (void *)ui->root);
	ui->root->data = ui;
	ambient_section = create_ambient_section(scene, g_v2f_zero);
	if (ambient_section)
	{
		printf("create_ui: Adding ambient section to UI\n");
		ui_add_child(ui->root, ambient_section);
	}
	else
	{
		printf("create_ui: Warning - Failed to create ambient section\n");
	}

	printf("create_ui: UI creation complete\n");
	return (ui);
}

void	destroy_ui(t_ui *ui)
{
	if (!ui)
		return;
	if (ui->root)
		free(ui->root);
	free(ui);
}

void render_ui(t_ui *ui)
{
	printf("render_ui: Starting UI render...\n");
	
	if (!ui) {
		printf("render_ui: Error - UI is NULL\n");
		return;
	}
	if (!ui->root)
	{
		printf("render_ui: Error - UI root is NULL\n");
		return;
	}
	if (!ui->ui_layer)
	{
		printf("render_ui: Error - UI layer is NULL\n");
		return;
	}
	printf("render_ui: UI Layer dimensions: %dx%d\n", 
		  ui->ui_layer->width, ui->ui_layer->height);
	printf("render_ui: Clearing UI layer...\n");
	memset(ui->ui_layer->pixels, 0, 
		  ui->ui_layer->width * ui->ui_layer->height * sizeof(int32_t));

	printf("render_ui: Rendering UI elements...\n");
	render_ui_element(ui->root, ui->ui_layer);
	
	printf("render_ui: UI render complete\n");
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(game->mlx);
	else if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
	{
		if (game->ui && game->ui->root)
		{
			game->ui->root->style.visible = !game->ui->root->style.visible;
			if (game->ui->ui_layer)
				game->ui->ui_layer->instances[0].enabled = game->ui->root->style.visible;
		}
	}
}

void mouse_hook(mouse_key_t button, action_t action, 
	__attribute__((unused)) modifier_key_t mods, void *param)
{
	t_game *game = param;
	int32_t x, y;

	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && game->ui && game->ui->root->style.visible)
	{
		mlx_get_mouse_pos(game->mlx, &x, &y);
		handle_ui_click(game->ui->root, x, y);
		render_ui(game->ui); 
	}
}

bool init_ui(t_game *game, t_scene *scene)
{
	game->ui = create_ui(game->mlx, scene);
	if (!game->ui)
	{
		printf("Error: Failed to create UI\n");
		return false;
	}
	if (mlx_image_to_window(game->mlx, game->ui->ui_layer, 0, 0) == -1)
	{
		printf("Error: Failed to add UI layer to window\n");
		destroy_ui(game->ui);
		game->ui = NULL;
		return false;
	}
	if (game->ui->root)
		game->ui->root->style.visible = true;
		
	game->ui->needs_redraw = true;
	return true;
}

void cleanup_mlx(t_game *game)
{
	if (!game)
		return;
	
	if (game->mlx)
	{
		if (game->scene && game->scene->camera.img_plane)
			mlx_delete_image(game->mlx, game->scene->camera.img_plane);
		
		if (game->ui)
		{
			if (game->ui->ui_layer)
				mlx_delete_image(game->mlx, game->ui->ui_layer);
			destroy_ui(game->ui);
			game->ui = NULL;
		}
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}

void game_cleanup(t_game *game)
{
	if (!game)
		return;
		
	if (game->ui) {
		destroy_ui(game->ui);
		game->ui = NULL;
	}
	
	if (game->mlx)
	{
		if (game->scene && game->scene->camera.img_plane)
			mlx_delete_image(game->mlx, game->scene->camera.img_plane);
		mlx_terminate(game->mlx);
	}
}

// Remove unused render_loop_wrapper

int game(t_scene *scene)
{
	t_game game;

	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
		return 1;
	game.scene = scene;
	scene->camera.img_plane = mlx_new_image(game.mlx, WIDTH, HEIGHT);
	if (!scene->camera.img_plane)
		return (cleanup_mlx(&game), 1);
	if (mlx_image_to_window(game.mlx, scene->camera.img_plane, 0, 0) == -1)
		return (cleanup_mlx(&game), 1);
	if (!init_ui(&game, scene))
		printf("Warning: Failed to initialize UI\n");
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);

	printf("Initial scene render...\n");
	render(scene);
	if (game.ui) {
		printf("Initial UI render...\n");
		printf("UI Layer address: %p\n", (void *)game.ui->ui_layer);
		printf("UI Root address: %p\n", (void *)game.ui->root);
		if (game.ui->ui_layer) {
			printf("UI Layer dimensions: %dx%d\n", 
				  game.ui->ui_layer->width, game.ui->ui_layer->height);
			if (game.ui->ui_layer->instances) {
				printf("Setting UI layer depth to 1\n");
				mlx_set_instance_depth(game.ui->ui_layer->instances, 1);
			} else {
				printf("Warning: UI layer has no instances!\n");
			}
		} else {
			printf("Warning: UI layer is NULL!\n");
		}
		if (scene->camera.img_plane && scene->camera.img_plane->instances) {
			printf("Setting scene layer depth to 0\n");
			mlx_set_instance_depth(scene->camera.img_plane->instances, 0);
		}
		printf("Forcing UI redraw...\n");
		game.ui->needs_redraw = true;
		render_ui(game.ui);
		mlx_image_to_window(game.mlx, scene->camera.img_plane, 0, 0);
		if (game.ui->ui_layer) {
			mlx_image_to_window(game.mlx, game.ui->ui_layer, 0, 0);
		}
	}
	printf("Entering main loop...\n");
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return 0;
}
