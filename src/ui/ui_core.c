/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:53:50 by bewong            #+#    #+#             */
/*   Updated: 2025/08/04 22:35:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdint.h>
#include <stdlib.h>



static void	(*const g_default_stylers[])(t_ui_element *, t_v2f, t_v2f) = {
	[UI_PANEL] = default_panel,
	[UI_BUTTON] = default_button,
	[UI_LABEL] = default_label,
	[UI_HEADER] = default_header,
	[UI_SECTION] = default_section,
};

/**
 * @brief Creates and initializes a new UI context
 * 
 * @param mlx The MLX instance to use for rendering
 * @param scene The scene data to be used by the UI
 * @return t_ui_context* The created UI context, or NULL on failure
 */
t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene)
{
	t_ui_context	*ctx;

	fprintf(stderr, "Creating UI context...\n");

	if (!mlx || !scene)
	{
		fprintf(stderr, "Error: MLX or scene is NULL\n");
		return (NULL);
	}

	fprintf(stderr, "Allocating UI context memory...\n");
	ctx = (t_ui_context *)ft_calloc(1, sizeof(t_ui_context));
	if (!ctx)
	{
		fprintf(stderr, "Error: Failed to allocate UI context memory\n");
		return (NULL);
	}
	fprintf(stderr, "Initializing UI context fields...\n");
	ctx->mlx = mlx;
	ctx->scene = scene;
	ctx->is_visible = true;
	ctx->needs_redraw = true;
	fprintf(stderr, "Creating UI canvas...\n");
	fprintf(stderr, "Window dimensions: %dx%d\n", mlx->width, mlx->height);
	fprintf(stderr, "UI Panel dimensions: %dx%d\n", UI_PANEL_WIDTH, HEIGHT);
	int panel_width = UI_PANEL_WIDTH;
	if (panel_width > mlx->width / 2) {
		panel_width = mlx->width / 2;
		fprintf(stderr, "Warning: UI panel too wide, resizing to %d\n", panel_width);
	}
	ctx->canvas = mlx_new_image(mlx, panel_width, mlx->height);
	if (!ctx->canvas)
	{
		fprintf(stderr, "Error: Failed to create UI canvas\n");
		free(ctx);
		return (NULL);
	}
	fprintf(stderr, "Canvas created successfully at %p\n", (void *)ctx->canvas);
	unsigned int *pixels = (unsigned int *)ctx->canvas->pixels;
	int i = 0;
	while (i < panel_width * mlx->height)
	{
		pixels[i] = 0x00000000;
		i++;
	}
	fprintf(stderr, "UI canvas initialized with transparency\n");

	// Add the canvas to the window (positioned on the left side)
	fprintf(stderr, "Adding canvas to window...\n");
	
	// Position the panel on the left side with no margin
	int x_pos = 0;
	
	// Ensure panel fits within the window
	if (panel_width > (int)mlx->width) {
		panel_width = mlx->width;
	}
	fprintf(stderr, "Canvas position: x=%d, y=0, width=%d, height=%d\n", 
		x_pos, panel_width, mlx->height);
	int instance_id = mlx_image_to_window(mlx, ctx->canvas, x_pos, 0);
	if (instance_id < 0) {
		fprintf(stderr, "Error: Failed to add canvas to window\n");
		mlx_delete_image(mlx, ctx->canvas);
		free(ctx);
		return (NULL);
	}

	// Canvas instance properties are set below after adding to window
	uint32_t y = 0;
	uint32_t x = 0;
	while (y < (uint32_t)mlx->height)
	{
		while (x < (uint32_t)panel_width)
		{
			uint32_t *pixel = (uint32_t *)ctx->canvas->pixels + y * panel_width + x;
			*pixel = 0x1A1A1AC8; // Dark semi-transparent background
			x++;
		}
		y++;
	}
	t_v2f border_pos = init_v2f(0, 0);
	t_v2f border_size = init_v2f(panel_width - 1, mlx->height - 1);
	draw_rect_border(ctx->canvas, border_pos, border_size, 0x333333FF);
	// Add canvas to window and set properties
	ctx->canvas_instance = mlx_image_to_window(mlx, ctx->canvas, x_pos, 0);
	if (ctx->canvas_instance < 0)
	{
		mlx_delete_image(mlx, ctx->canvas);
		free(ctx);
		return NULL;
	}

	// Set canvas properties
	ctx->canvas->instances[0].z = 1000;
	ctx->canvas->instances[0].enabled = true;
	mlx_set_instance_depth(&ctx->canvas->instances[0], 1000);
	if (ctx->canvas_instance >= 0 && ctx->canvas->count > 0)
	{
		t_v2f pos, size;
		ctx->canvas->instances[0].z = 1000;
		ctx->canvas->instances[0].enabled = true;
		uint32_t y = 0;
		while (y < ctx->canvas->height)
		{
			uint32_t x = 0;
			while (x < ctx->canvas->width)
			{
				uint32_t *pixel = (uint32_t *)ctx->canvas->pixels + y * ctx->canvas->width + x;
				*pixel = (*pixel & 0x00FFFFFF) | 0xFF000000;
				x++;
			}
			y++;
		}
		pos = init_v2f(0, 0);
		size = init_v2f((float)ctx->canvas->width, (float)ctx->canvas->height);
		draw_rect_border(ctx->canvas, pos, size, 0x00FF00FF);
	}
	ctx->needs_redraw = true;
	ctx->images = (t_ui_images *)ft_calloc(1, sizeof(t_ui_images));
	if (!ctx->images)
	{
		mlx_delete_image(mlx, ctx->canvas);
		free(ctx);
		return (NULL);
	}
	return (ctx);
}

static void	ui_images_destroy(mlx_t *mlx, t_ui_images *images)
{
	if (!images)
		return;
	if (mlx)
	{
		if (images->button_img)
			mlx_delete_image(mlx, images->button_img);
		if (images->header_img)
			mlx_delete_image(mlx, images->header_img);
		if (images->panel_img)
			mlx_delete_image(mlx, images->panel_img);
	}
	free(images);
}

void	destroy_ui_context(t_ui_context *ctx)
{
	if (!ctx)
		return;
	if (ctx->canvas)
		mlx_delete_image(ctx->mlx, ctx->canvas);
	if (ctx->images)
		ui_images_destroy(ctx->mlx, ctx->images);
	free(ctx);
}

void	destroy_ui(t_ui *ui)
{
	if (!ui)
		return;
	if (ui->root)
		destroy_ui_element_recursive(ui->root, ui->context, true);
	if (ui->context)
		destroy_ui_context(ui->context);
	free(ui);
}

void	toggle_ui_visibility(t_ui *ui)
{
	if (!ui || !ui->context) {
		fprintf(stderr, "Error: Cannot toggle UI visibility - invalid UI or context\n");
		return;
	}
	t_ui_context *ctx = ui->context;
	ctx->is_visible = !ctx->is_visible;

	fprintf(stderr, "UI visibility toggled to: %s\n", 
		ctx->is_visible ? "VISIBLE" : "HIDDEN");
}

void	render_ui(t_ui *ui)
{
	if (!ui || !ui->context || !ui->context->canvas || !ui->root) {
		fprintf(stderr, "Warning: Cannot render UI - missing context, canvas, or root element\n");
		return;
	}

	t_ui_context *ctx = ui->context;
	mlx_image_t *canvas = ctx->canvas;
	int32_t x_pos = 0;
	if (!ctx->is_visible)
	{
		size_t i = 0;
		while (i < canvas->count)
		{
			canvas->instances[i].enabled = false;
			i++;
		}
		fprintf(stderr, "Hiding UI canvas (disabled %zu instances)\n", canvas->count);
		return;
	}
	if (canvas->count == 0)
	{
		int instance_id = mlx_image_to_window(ctx->mlx, canvas, x_pos, 0);
		if (instance_id < 0)
		{
			fprintf(stderr, "ERROR: Failed to add canvas instance\n");
			return;
		}
		canvas->instances[0].enabled = true;
		canvas->instances[0].z = 1000;
		mlx_set_instance_depth(&canvas->instances[0], 1000);
	}
	size_t i = 0;
	while (i < canvas->count)
	{
		canvas->instances[i].enabled = true;
		canvas->instances[i].x = x_pos;
		canvas->instances[i].y = 0;
		i++;
	}
	uint32_t y = 0;
	while (y < canvas->height)
	{
		uint32_t x = 0;
		while (x < canvas->width)
		{
			uint32_t *pixel = (uint32_t *)canvas->pixels + y * canvas->width + x;
			*pixel = 0x00000000; // Transparent black
			x++;
		}
		y++;
	}
	if (canvas->count > 0)
	{
		canvas->instances[0].enabled = true;
		canvas->instances[0].z = 1000;
		mlx_set_instance_depth(&canvas->instances[0], 1000);
	}
	render_ui_element(ui->root, ctx);
	ctx->needs_redraw = true;
}

t_ui_element	*ui_element_create(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->data = NULL;
	element->action = NULL;
	element->visible = true;
	if (type < (sizeof(g_default_stylers) / sizeof(g_default_stylers[0])) && 
		g_default_stylers[type] != NULL)
		g_default_stylers[type](element, pos, size);
	return (element);
}

void	attach_child(t_ui_element *parent, t_ui_element *child)
{
	if (!parent || !child)
		return;
	child->parent = parent;
	child->next_sibling = parent->first_child;
	parent->first_child = child;
}
