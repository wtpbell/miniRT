/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_update_animation.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 14:48:40 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 14:48:56 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	update_animation(t_ani *ani, t_sprite *bg, float delta)
{
	t_sprite	*frame;
	uint32_t	next_idx;

	ani->time += delta * ani->fps;
	next_idx = (int)ani->time % ani->frame_count;
	if (next_idx != ani->idx)
		ani->idx = next_idx;
	frame = ani->frames + next_idx;
	draw_frame(bg->img, frame->img,
		get_sprite_position(bg, frame, ani->pos));
}
