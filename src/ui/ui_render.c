/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:03:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/20 18:02:20 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_thread.h"

uint32_t	blend_colors(uint32_t bg, uint32_t fg)
{
	float		alpha;
	float		inv_alpha;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;

	alpha = (fg & 0xFF) / 255.0f;
	inv_alpha = 1.0f - alpha;
	r = ((bg >> 24) & 0xFF) * inv_alpha + ((fg >> 24) & 0xFF) * alpha;
	g = ((bg >> 16) & 0xFF) * inv_alpha + ((fg >> 16) & 0xFF) * alpha;
	b = ((bg >> 8) & 0xFF) * inv_alpha + ((fg >> 8) & 0xFF) * alpha;
	return ((r << 24) | (g << 16) | (b << 8) | 0xFFu);
}

void	render_ui_element(t_ui_element *e, t_ui_context *c)
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

void	render_loop(void *param)
{
	t_game			*game;
	t_ui_context	*ctx;

	game = (t_game *)param;
	if (game->needs_redraw)
	{
		ft_memset(game->img->pixels, 0, game->img->width
			* game->img->height * sizeof(int));
		if (thread_rendering(game->scene, game->sample))
		{
			game->img->instances[0].enabled = true;
			game->needs_redraw = false;
		}
	}
	if (game->ui && game->ui->context)
	{
		ctx = game->ui->context;
		if (ctx->is_visible && ctx->canvas)
			render_ui(game->ui);
	}
}
