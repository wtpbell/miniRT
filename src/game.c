/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 11:50:39 by jboon             #+#    #+#             */
/*   Updated: 2025/07/25 00:33:47 by bewong           ###   ########.fr       */
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

	// Only render if UI exists and needs redraw
	if (game->ui && game->ui->needs_redraw) {
		render_ui(game->ui);
		game->ui->needs_redraw = false;
	}
}

#define WIDTH    1600
#define HEIGHT   900

t_ui	*create_ui(mlx_t *mlx, t_scene *scene)
{
	t_ui				*ui = NULL;
	t_ui_element	*ambient_section = NULL;
	t_v2f			root_pos;
	t_v2f			root_size;
	t_v2f			ambient_pos;

	printf("create_ui: Starting UI creation\n");
	printf("create_ui: MLX pointer: %p\n", (void *)mlx);
	printf("create_ui: Scene pointer: %p\n", (void *)scene);
	if (!mlx || !scene)
	{
		printf("create_ui: Error - MLX or Scene pointer is NULL\n");
		return (NULL);
	}
	printf("create_ui: Allocating UI structure...\n");
	ui = ft_calloc(1, sizeof(t_ui));
	if (!ui)
	{
		printf("create_ui: Error - Failed to allocate UI\n");
		return (NULL);
	}
	ui->mlx = mlx;
	ui->needs_redraw = true;
	printf("create_ui: Creating UI images...\n");
	ui->images = ui_images_create(mlx);
	if (!ui->images)
	{
		printf("create_ui: Error - Failed to create UI images\n");
		free(ui);
		return (NULL);
	}
	printf("create_ui: Creating root panel...\n");
	root_pos = init_v2f(0, 0);
	root_size = init_v2f(UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(mlx, root_pos, root_size);
	if (!ui->root)
	{
		printf("create_ui: Error - Failed to create root panel\n");
		destroy_ui(ui);
		return (NULL);
	}
	printf("create_ui: Root panel created at %p\n", (void *)ui->root);
	printf("create_ui: Creating ambient section...\n");
	ambient_pos = init_v2f(10, 10);
	t_v2f ambient_size = init_v2f(UI_PANEL_WIDTH - 20, 200); // 20px padding on sides
	ambient_section = create_ambient_section(mlx, scene, ambient_pos, ambient_size);
	if (!ambient_section)
	{
		printf("create_ui: Error - Failed to create ambient section\n");
		destroy_ui(ui);
		return (NULL);
	}
	printf("create_ui: Ambient section created at %p\n", (void *)ambient_section);
	printf("create_ui: Adding ambient section to root panel...\n");
	ui_add_child(ui->root, ambient_section);
	ui->needs_redraw = true;
	printf("create_ui: Rendering UI...\n");
	render_ui(ui);
	printf("create_ui: UI created successfully at %p\n", (void *)ui);
	return (ui);
}

void destroy_ui_element_recursive(t_ui_element *element, t_ui *ui)
{
	t_ui_element *child = element->first_child;
	while (child)
	{
		t_ui_element *next = child->next_sibling;
		destroy_ui_element_recursive(child, ui);
		child = next;
	}
	if (element->data)
	{
		if (element->type == UI_VALUE_BUTTON || element->type == UI_BUTTON)
		{
			t_ui_button *button = (t_ui_button *)element->data;
			if (button->label_text)
				free(button->label_text);
			free(button);
		}
		if (element->type == UI_LABEL)
		{
			t_ui_label *label = (t_ui_label *)element->data;
			if (label->text)
				free(label->text);
			free(label);
		}
		if (element->type == UI_PANEL || element->type == UI_HEADER || element->type == UI_SECTION)
			free(element->data);
		element->data = NULL;
	}
	if (element->image && ui && ui->mlx)
	{
		if (ui->images)
		{
			if (element->image == ui->images->button_img ||
				element->image == ui->images->header_img ||
				element->image == ui->images->panel_img)
			{
				element->image = NULL;
				return;
			}
		}
		mlx_delete_image(ui->mlx, element->image);
		element->image = NULL;
	}
	free(element);
}

void destroy_ui(t_ui *ui)
{
	if (!ui)
		return;
	
	printf("Destroying UI...\n");
	if (ui->root)
	{
		t_ui_element *element = ui->root;
		while (element)
		{
			element->image = NULL;
			if (element->first_child)
				element = element->first_child;
			else if (element->next_sibling)
				element = element->next_sibling;
			else
			{
				while (element->parent && !element->next_sibling)
					element = element->parent;
				if (element->parent)
					element = element->next_sibling;
				else
					break;
			}
		}
		
		if (ui->images)
		{
			ui_images_destroy(ui->mlx, ui->images);
			ui->images = NULL;
		}
	}
	ft_bzero(ui, sizeof(t_ui));
	free(ui);
	printf("UI destroyed successfully\n");
}

static void render_ui_element_recursive(t_ui_element *element, mlx_t *mlx, bool force_redraw)
{
	if (!element || !element->style.visible)
		return;

	bool needs_redraw = force_redraw || !element->image;
	if (!element->image)
	{
		element->image = mlx_new_image(mlx, element->size.x, element->size.y);
		if (!element->image)
		{
			printf("Failed to create image for UI element\n");
			return;
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
		int32_t depth = 100;
		if (element->type == UI_PANEL) depth = 10;
		else if (element->type == UI_SECTION) depth = 20;
		else if (element->type == UI_HEADER) depth = 30;
		else if (element->type == UI_BUTTON || element->type == UI_VALUE_BUTTON) depth = 40;
		
		mlx_set_instance_depth(&element->image->instances[element->instance_id], depth);
	}
	if (needs_redraw)
	{
		ui_element_render(element);
	}
	if (element->image && element->instance_id >= 0 && 
		element->instance_id < (int32_t)element->image->count)
	{
		element->image->instances[element->instance_id].x = (int)element->pos.x;
		element->image->instances[element->instance_id].y = (int)element->pos.y;
		element->image->instances[element->instance_id].enabled = element->style.visible && element->visible;
	}
	t_ui_element *child = element->first_child;
	while (child)
	{
		render_ui_element_recursive(child, mlx, needs_redraw);
		child = child->next_sibling;
	}
}

void render_ui(t_ui *ui)
{
	if (!ui || !ui->root || !ui->mlx)
	{
		printf("render_ui: Invalid parameters\n");
		return;
	}
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
		mlx_close_window(game->mlx);
	else if (keydata.key == MLX_KEY_H && keydata.action == MLX_PRESS)
	{
		if (game->ui && game->ui->root)
		{
			game->ui->root->style.visible = !game->ui->root->style.visible;
			if (game->ui->root)
				game->ui->root->visible = !game->ui->root->visible;
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
	if (!game || !game->mlx || !scene)
	{
		printf("Error: Invalid parameters for UI initialization\n");
		return false;
	}

	printf("Initializing UI...\n");
	game->ui = create_ui(game->mlx, scene);
	if (!game->ui)
	{
		printf("Error: Failed to create UI\n");
		return false;
	}
	if (!game->ui->root)
	{
		printf("Error: UI root panel is NULL\n");
		destroy_ui(game->ui);
		game->ui = NULL;
		return false;
	}
	game->ui->root->style.visible = true;
	game->ui->root->visible = true;
	game->ui->root->style.bg_color = 0x333333FF; // Dark gray background
	game->ui->root->style.border_color = 0x444444FF; // Slightly lighter border
	game->ui->root->style.text_color = 0xFFFFFFFF; // White text
	game->ui->root->style.padding = 4;
	
	printf("Root panel created at (%.0f, %.0f) size %.0fx%.0f\n", 
		   game->ui->root->pos.x, game->ui->root->pos.y,
		   game->ui->root->size.x, game->ui->root->size.y);
	game->ui->needs_redraw = true;
	render_ui(game->ui);
	if (!game->ui->root->image || game->ui->root->instance_id < 0)
	{
		printf("Error: Failed to add root panel to window\n");
		destroy_ui(game->ui);
		game->ui = NULL;
		return false;
	}
	if (game->ui->root->image && game->ui->root->instance_id >= 0)
	{
		mlx_set_instance_depth(
			&game->ui->root->image->instances[game->ui->root->instance_id], 
			1
		);
	}
	game->ui->root->style.visible = true;
	game->ui->needs_redraw = true;
	return true;
}

void cleanup_mlx(t_game *game)
{
	if (game->mlx)
	{
		if (game->scene && game->scene->camera.img_plane)
			mlx_delete_image(game->mlx, game->scene->camera.img_plane);
		if (game->ui)
		{
			destroy_ui(game->ui);
			game->ui = NULL;
		}
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
}

int game(t_scene *scene)
{
	t_game game;

	ft_bzero(&game, sizeof(t_game));
	game.scene = scene;
	game.mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!game.mlx)
	{
		printf("Error: Failed to initialize MLX\n");
		return 1;
	}
	scene->camera.img_plane = mlx_new_image(game.mlx, WIDTH, HEIGHT);
	if (!scene->camera.img_plane)
	{
		printf("Error: Failed to create render image\n");
		return (cleanup_mlx(&game), 1);
	}
	if (mlx_image_to_window(game.mlx, scene->camera.img_plane, 0, 0) == -1)
	{
		printf("Error: Failed to add render image to window\n");
		return (cleanup_mlx(&game), 1);
	}
	mlx_set_instance_depth(scene->camera.img_plane->instances, 0);
	if (!init_ui(&game, scene))
	{
		printf("Warning: UI initialization had issues, continuing without UI\n");
	}
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_mouse_hook(game.mlx, mouse_hook, &game);
	render(scene);
	printf("Entering main loop...\n");
	mlx_loop(game.mlx);
	cleanup_mlx(&game);
	return 0;
}
