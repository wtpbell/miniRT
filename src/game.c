/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/07/27 23:13:59 by bewong           ###   ########.fr       */
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


/**
 * @brief Main render loop for the game
 * 
 * @param param Pointer to the game structure
 */
void render_loop(void *param)
{
	t_game *game = (t_game *)param;

	if (!game->needs_redraw)
		return;

	// Clear the screen
	ft_memset(game->img->pixels, 0, game->img->width * game->img->height * sizeof(int));

	// Render 3D scene
	render(game->scene);

	// Render UI if it exists and needs redraw
	if (game->ui && game->ui->needs_redraw) {
		render_ui(game->ui);
		game->ui->needs_redraw = false;
	}

	game->needs_redraw = false;
}

/**
 * @brief Create the main UI structure
 * 
 * @param mlx MLX context
 * @param scene Scene data
 * @return t_ui* Pointer to the created UI structure
 */
t_ui *create_ui(mlx_t *mlx, t_scene *scene)
{
	t_ui *ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
	{
		fprintf(stderr, "Error: Failed to allocate UI structure\n");
		return (NULL);
	}

	ui->mlx = mlx;
	ui->needs_redraw = true;

	// Create root panel
	t_v2f root_pos = g_v2f_zero;
	t_v2f root_size = init_v2f(UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(mlx, root_pos, root_size);
	if (!ui->root)
	{
		fprintf(stderr, "Error: Failed to create root panel\n");
		free(ui);
		return (NULL);
	}

	// Create and add ambient section
	t_v2f ambient_pos = init_v2f(10, 10);
	t_v2f ambient_size = init_v2f(UI_PANEL_WIDTH - 20, 200);
	t_ui_element *ambient_section = create_ambient_section(mlx, scene, ambient_pos, ambient_size);
	if (!ambient_section)
	{
		fprintf(stderr, "Error: Failed to create ambient section\n");
		ui_element_destroy(ui->root, mlx, true);
		free(ui);
		return (NULL);
	}

	// Add the ambient section to the root panel
	attach_child(ui->root, ambient_section);
	ui->needs_redraw = true;

	// Initial render - We'll render the UI in the render_loop
	// No need to render here as it will be handled by the render_loop
	
	return (ui);
}

// UI destruction functions have been moved to ui_utils.c to avoid duplication

void destroy_ui(t_ui *ui)
{
	if (!ui)
		return;

	// Destroy the UI element tree using the implementation in ui_utils.c
	if (ui->root)
	{
		destroy_ui_element_recursive(ui->root, ui);
		ui->root = NULL;
	}

	// Clean up any remaining resources
	if (ui->mlx && ui->images)
	{
		ui_images_destroy(ui->mlx, ui->images);
		ui->images = NULL;
	}

	free(ui);
}

void	render_ui_element_recursive(t_ui_element *element, mlx_t *mlx, bool force_redraw)
{
	bool			needs_redraw;
	int32_t			depth;
	t_ui_element	*child;

	needs_redraw = force_redraw || !element->image;
	if (!element->image)
	{
		element->image = mlx_new_image(mlx, element->size.x, element->size.y);
		if (!element->image)
		{
			printf("Failed to create image for UI element\n");
			return ;
		}
		needs_redraw = true;
		element->instance_id = mlx_image_to_window(mlx, element->image, 
												(int)element->pos.x, (int)element->pos.y);
		if (element->instance_id == -1)
		{
			printf("Failed to add UI element to window\n");
			mlx_delete_image(mlx, element->image);
			element->image = NULL;
			return;
		}
		depth = 100;
		if (element->type == UI_PANEL) depth = 10;
		else if (element->type == UI_SECTION) depth = 20;
		else if (element->type == UI_HEADER) depth = 30;
		else if (element->type == UI_BUTTON || element->type == UI_VALUE_BUTTON) depth = 40;
		mlx_set_instance_depth(&element->image->instances[element->instance_id], depth);
	}
	if (needs_redraw)
		render_ui_element(element, NULL);
	if (element->image && element->instance_id >= 0 && 
		element->instance_id < (int32_t)element->image->count)
	{
		element->image->instances[element->instance_id].x = (int)element->pos.x;
		element->image->instances[element->instance_id].y = (int)element->pos.y;
		element->image->instances[element->instance_id].enabled = element->style.visible && element->visible;
	}
	child = element->first_child;
	while (child)
	{
		render_ui_element_recursive(child, mlx, needs_redraw);
		child = child->next_sibling;
	}
}

void	render_ui(t_ui *ui)
{
	render_ui_element_recursive(ui->root, ui->mlx, ui->needs_redraw);
	ui->needs_redraw = false;
	printf("UI render complete - Root element at (%.0f,%.0f) size %.0fx%.0f\n", 
		   ui->root->pos.x, ui->root->pos.y, 
		   ui->root->size.x, ui->root->size.y);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
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
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && game->ui
		&& game->ui->root->style.visible)
	{
		mlx_get_mouse_pos(game->mlx, &x, &y);
		handle_ui_click(game->ui->root, x, y);
		render_ui(game->ui);
	}
}

void cleanup_mlx(t_game *game)
{
	if (!game) return;

	// First, clean up UI elements that might be using MLX resources
	if (game->ui)
	{
		if (game->ui->root)
		{
			destroy_ui_element_recursive(game->ui->root, game->ui);
			game->ui->root = NULL;
		}

		// Clean up MLX images and UI images
		if (game->ui->images && game->mlx)
		{
			ui_images_destroy(game->mlx, game->ui->images);
			game->ui->images = NULL;
		}

		// Free the UI structure
		free(game->ui);
		game->ui = NULL;
	}

	// Finally, terminate MLX
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
	game->ui = create_ui(game->mlx, scene);
	if (!game->ui)
	{
		fprintf(stderr, "Error: Failed to initialize UI\n");
		return (false);
	}
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
	if (!cam_init(&scene->camera, game.mlx))
	{
		fprintf(stderr, "Error: Failed to initialize camera\n");
		cleanup_mlx(&game);
		return (EXIT_FAILURE);
	}
	mlx_loop_hook(game.mlx, render_loop, &game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	if (!init_ui(&game, scene))
		fprintf(stderr, "Warning: UI initialization had issues, continuing without UI\n");
	render(scene);
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return (EXIT_SUCCESS);
}
