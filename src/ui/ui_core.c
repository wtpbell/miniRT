/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:53:50 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 11:10:10 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdint.h>
#include <stdlib.h>

static t_ui_images	*init_ui_images(void)
{
	t_ui_images	*images;

	images = ft_calloc(1, sizeof(t_ui_images));
	if (!images)
		return (NULL);
	images->button_img = NULL;
	images->header_img = NULL;
	images->panel_img = NULL;
	return (images);
}

static void	init_ui_context(t_ui_context *ctx, mlx_t *mlx,
						t_scene *scene, void *game_ptr)
{
	if (!ctx)
		return ;
	ctx->mlx = mlx;
	ctx->scene = scene;
	ctx->game = game_ptr;
	ctx->is_visible = true;
	ctx->needs_redraw = true;
}

t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene, void *game_ptr)
{
	t_ui_context	*ctx;
	int				panel_width;

	ctx = (t_ui_context *)ft_calloc(1, sizeof(t_ui_context));
	if (!ctx)
		return (NULL);
	init_ui_context(ctx, mlx, scene, game_ptr);
	panel_width = UI_PANEL_WIDTH;
	if (panel_width > mlx->width / 2)
		panel_width = mlx->width / 2;
	ctx->images = init_ui_images();
	if (!ctx->images)
		return (free(ctx), NULL);
	ctx->canvas = mlx_new_image(mlx, panel_width, mlx->height);
	if (!ctx->canvas)
		return (destroy_ui_context(ctx), NULL);
	if (panel_width > (int)mlx->width)
		panel_width = mlx->width;
	if (mlx_image_to_window(mlx, ctx->canvas, 0, 0) < 0)
		return (destroy_ui_context(ctx), NULL);
	return (ctx);
}

void	toggle_ui_visibility(t_ui *ui)
{
	size_t			i;
	t_ui_context	*ctx;

	if (!ui || !ui->context || !ui->context->canvas)
		return ;
	ctx = ui->context;
	ctx->is_visible = !ctx->is_visible;
	i = 0;
	while (i < ctx->canvas->count)
	{
		ctx->canvas->instances[i].enabled = ctx->is_visible;
		i++;
	}
}

static void	attach_canvas_to_window(t_ui_context *ctx)
{
	mlx_image_t	*canvas;

	canvas = ctx->canvas;
	if (canvas->count == 0 && \
		mlx_image_to_window(ctx->mlx, canvas, 0, 0) >= 0)
	{
		canvas->instances[0].z = 1000;
		canvas->instances[0].enabled = true;
	}
}

static void	set_canvas_visibility(mlx_image_t *canvas, bool visible)
{
	uint32_t	i;

	i = 0;
	while (i < canvas->count)
		canvas->instances[i++].enabled = visible;
}

void	render_ui(t_ui *ui)
{
	t_ui_context	*ctx;
	uint32_t		*pixels;
	uint32_t		i;

	ctx = ui->context;
	attach_canvas_to_window(ctx);
	set_canvas_visibility(ctx->canvas, ctx->is_visible);
	if (!ctx->is_visible || !ctx->needs_redraw)
		return ;
	pixels = (uint32_t *)ctx->canvas->pixels;
	i = 0;
	while (i < ctx->canvas->width * ctx->canvas->height)
		pixels[i++] = UI_PANEL_BG_COLOR;
	if (ui->root)
		render_ui_element(ui->root, ctx);
	ctx->needs_redraw = false;
}

void	attach_child(t_ui_element *parent, t_ui_element *child)
{
	if (!parent || !child)
		return ;
	child->parent = parent;
	child->next_sibling = parent->first_child;
	parent->first_child = child;
}
