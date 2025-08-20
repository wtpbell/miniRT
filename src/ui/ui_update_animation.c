/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_update_animation.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 14:48:40 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/20 21:11:01 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

t_v2i	get_sprite_position(t_sprite *parent, t_sprite *child, t_v2i pos)
{
	return ((t_v2i){
		.x = (parent->full_size.x * parent->anchor.x)
		- (child->full_size.x * child->anchor.x) + pos.x * child->scale.x,
		.y = (parent->full_size.y * parent->anchor.y)
		- (child->full_size.y * child->anchor.y) + pos.y * child->scale.y
	});
}

void	init_sprite_full_size(t_sprite *sprite)
{
	sprite->full_size = init_v2i(
		sprite->img->width * sprite->scale.x,
		sprite->img->height * sprite->scale.y
	);
}

void	update_animation(t_ani *ani, t_sprite *bg, float delta)
{
	t_sprite	*frame;
	uint32_t	next_idx;

	ani->time += delta * ani->fps;
	next_idx = (int)ani->time % ani->frame_count;
	if (next_idx != ani->idx)
		ani->idx = next_idx;
	frame = ani->frames + next_idx;

	// TODO: clean up
	t_v2i	posi = get_sprite_position(bg, frame, ani->pos);
	t_v2f	size = init_v2f(frame->img->width * frame->scale.x, frame->img->height * frame->scale.y);
	t_v2f	posf = init_v2f(posi.x, posi.y);
	// draw_rect(bg->img, posf, size, C_RED);
	
	draw_rect(bg->img, posf, size, C_BLUE);
	draw_frame(bg, frame, get_sprite_position(bg, frame, ani->pos));

	draw_rect(bg->img,
		init_v2f(0, bg->img->height * bg->anchor.y),
		init_v2f(bg->img->width, 1),
		C_RED
	);

	draw_rect(bg->img,
		init_v2f(bg->img->width * bg->anchor.x, 0),
		init_v2f(1, bg->img->height),
		C_RED
	);
}
