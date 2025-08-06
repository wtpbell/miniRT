/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:53:50 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 12:38:54 by bewong           ###   ########.fr       */
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


t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene)
{
	t_ui_context	*ctx;
	unsigned int	*pixels;
	int				i;
	int				x_pos;
	int				panel_width;

	ctx = (t_ui_context *)ft_calloc(1, sizeof(t_ui_context));
	if (!ctx)
		return (NULL);
	ctx->mlx = mlx;
	ctx->scene = scene;
	ctx->is_visible = true;
	ctx->needs_redraw = true;
	panel_width = UI_PANEL_WIDTH;
	if (panel_width > mlx->width / 2)
		panel_width = mlx->width / 2;
	ctx->canvas = mlx_new_image(mlx, panel_width, mlx->height);
	if (!ctx->canvas)
		return (free(ctx), NULL);
	pixels = (unsigned int *)ctx->canvas->pixels;
	i = 0;
	while (i < panel_width * mlx->height)
	{
		pixels[i] = UI_TRANSPARENT;
		i++;
	}
	x_pos = 0;
	if (panel_width > (int)mlx->width)
		panel_width = mlx->width;
	if (mlx_image_to_window(mlx, ctx->canvas, x_pos, 0) < 0)
		return (mlx_delete_image(mlx, ctx->canvas), free(ctx), NULL);
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
	size_t			i;
	t_ui_context	*ctx;

	if (!ui || !ui->context || !ui->context->canvas)
		return;
		
	ctx = ui->context;
	ctx->is_visible = !ctx->is_visible;
	i = 0;
	while (i < ctx->canvas->count)
	{
		ctx->canvas->instances[i].enabled = ctx->is_visible;
		i++;
	}
	fprintf(stderr, "UI visibility toggled to: %s\n", 
		ctx->is_visible ? "VISIBLE" : "HIDDEN");
}

void	render_ui(t_ui *ui)
{
	t_ui_context	*ctx;
	mlx_image_t		*canvas;
	uint32_t		*pixels;
	uint32_t		i;

	if (!ui || !ui->context)
		return;
	ctx = ui->context;
	canvas = ctx->canvas;
	if (!canvas)
		return;

	if (canvas->count == 0)
	{
		if (mlx_image_to_window(ctx->mlx, canvas, 0, 0)< 0)
		{
			fprintf(stderr, "Failed to add canvas to window\n");
			return ;
		}
		canvas->instances[0].z = 1000;
		canvas->instances[0].enabled = true;
	}
	if (!ctx->is_visible)
	{
		i = 0;
		while (i < canvas->count)
		{
			canvas->instances[i].enabled = false;
			i++;
		}
		return ;
	} 
	else
	{
		i = 0;
		while (i < canvas->count)
		{
			canvas->instances[i].enabled = true;
			i++;
		}
	}
	if (!ctx->needs_redraw)
		return ;
	pixels = (uint32_t *)canvas->pixels;
	i = 0;
	while (i < canvas->width * canvas->height)
	{
		pixels[i] = UI_TRANSPARENT;
		i++;
	}
	if (ui->root)
		render_ui_element(ui->root, ctx);
	ctx->needs_redraw = false;
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
