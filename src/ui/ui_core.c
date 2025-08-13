/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_core.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 12:53:50 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/13 22:12:44 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	set_canvas_visibility(mlx_image_t *canvas, bool visible)
{
	uint32_t	i;

	i = 0;
	while (i < canvas->count)
		canvas->instances[i++].enabled = visible;
}

static void	attach_canvas_to_window(t_ui_context *ctx)
{
	mlx_image_t	*canvas;

	canvas = ctx->canvas;
	if (canvas->count == 0 && mlx_image_to_window(ctx->mlx, canvas, 0, 0) >= 0)
	{
		canvas->instances[0].z = 1000;
		canvas->instances[0].enabled = true;
	}
}

void	render_ui(t_ui *ui)
{
	t_ui_context	*ctx;
	uint32_t		*pixels;
	uint32_t		i;

	if (!ui || !ui->context || !ui->context->canvas)
		return ;
	ctx = ui->context;
	attach_canvas_to_window(ctx);
	if (!ctx->is_visible)
	{
		set_canvas_visibility(ctx->canvas, false);
		return ;
	}
	set_canvas_visibility(ctx->canvas, true);
	if (!ctx->needs_redraw && !ctx->is_dirty)
		return ;
	pixels = (uint32_t *)ctx->canvas->pixels;
	i = 0;
	while (i < ctx->canvas->width * ctx->canvas->height)
		pixels[i++] = UI_PANEL_BG_COLOR;
	if (ui->root)
		render_ui_element(ui->root, ctx);
	ctx->is_dirty = false;
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
