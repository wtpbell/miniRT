/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_core.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 12:53:50 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/18 11:09:59 by jboon         ########   odam.nl         */
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

static void	render_ui_element(t_ui_element *e, t_ui_context *c)
{
	t_ui_element	*child;

	if (!e->visible)
		return ;
	e->abs_pos = e->pos;
	if (e->parent)
		e->abs_pos = v2f_add(e->parent->abs_pos, e->pos);
	if (e->type == UI_PANEL || e->type == UI_HEADER)
	{
		draw_rect(c->canvas, e->abs_pos, e->size, e->style.bg_color);
		draw_rect_border(c->canvas, e->abs_pos, e->size, e->style.border_color);
	}
	else if (e->type == UI_BUTTON)
		draw_button(e, c);
	else if (e->type == UI_LABEL)
		draw_text(c->canvas, ((t_ui_label *)e->data)->text, e->abs_pos,
			e->style.text_color);
	child = e->first_child;
	while (child && (render_ui_element(child, c), 1))
		child = child->next_sibling;
}

void	render_ui(t_ui *ui)
{
	t_ui_context	*ctx;
	uint32_t		*pixels;
	uint32_t		i;

	ctx = ui->context;
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
