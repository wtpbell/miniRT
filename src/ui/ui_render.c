/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:03:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 15:53:21 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "color.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void	put_pixel_if_visible(mlx_image_t *c, t_v2f p, uint32_t col)
{
	uint32_t	*pix;

	if (p.x >= 0 && p.y >= 0 && p.x < c->width && p.y < c->height)
	{
		if ((col >> 24) != 0x00)
		{
			pix = (uint32_t *)c->pixels + (int)p.y * c->width + (int)p.x;
			*pix = col;
		}
	}
}

uint32_t	blend_colors(uint32_t bg, uint32_t fg)
{
	float	alpha;
	float	inv_alpha;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	alpha = ((fg >> 24) & 0xFF) / 255.0f;
	inv_alpha = 1.0f - alpha;
	r = (uint8_t)(((bg >> 16) & 0xFF) * inv_alpha
			+ ((fg >> 16) & 0xFF) * alpha);
	g = (uint8_t)(((bg >> 8) & 0xFF) * inv_alpha
			+ ((fg >> 8) & 0xFF) * alpha);
	b = (uint8_t)((bg & 0xFF) * inv_alpha + (fg & 0xFF) * alpha);
	return ((0xFFu << 24) | (r << 16) | (g << 8) | b);
}

void	render_ui_element(t_ui_element *e, t_ui_context *ctx)
{
	if (!e->visible)
		return ;
	e->abs_pos = e->pos;
	if (e->parent)
	{
		e->abs_pos.x += e->parent->abs_pos.x + e->parent->style.padding;
		e->abs_pos.y += e->parent->abs_pos.y + e->parent->style.padding;
	}
	if (e->type == UI_PANEL || e->type == UI_HEADER)
	{
		draw_rect(ctx->canvas, e->abs_pos, e->size, e->style.bg_color);
		draw_rect_border(ctx->canvas, e->abs_pos,
			e->size, e->style.border_color);
	}
	else if (e->type == UI_BUTTON)
		draw_button(e, ctx);
	else if (e->type == UI_LABEL)
		draw_text(ctx->canvas, ((t_ui_label *)e->data)->text,
			e->abs_pos, e->style.text_color);
	while (e->first_child)
	{
		render_ui_element(e->first_child, ctx);
		e->first_child = e->first_child->next_sibling;
	}
}
