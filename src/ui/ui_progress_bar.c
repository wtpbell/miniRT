/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_progress_bar.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/17 21:24:42 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 23:24:37 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

bool	init_progress_bar(t_progress_bar *pb, t_v2i pos, t_v2i size, mlx_t *mlx)
{
	pb->pos = pos;
	pb->size = size;
	pb->text.anchor = init_v2f(0.5f, 0.5f);
	pb->text.img = init_str_frame(mlx, "100.00%");
	if (pb->text.img == NULL)
		return (false);
	pb->bg.anchor = init_v2f(0.5f, 0.5f);
	pb->bg.img = mlx_new_image(mlx, size.x, size.y);
	if (pb->bg.img == NULL)
		return (mlx_delete_image(mlx, pb->text.img), false);
	pb->bg_color = C_BLUE;
	pb->bar_color = C_RED;
	return (true);
}

void	update_progress_bar(t_progress_bar *pb, t_sprite *bg, float t)
{
	const t_v2f	pos = init_v2f(pb->pos.x, pb->pos.y);
	const t_v2f	size = init_v2f(pb->size.x, pb->size.y);
	char		buf[16];

	draw_rect(pb->bg.img, pos, size, pb->bg_color);
	draw_rect(pb->bg.img, pos, v2f_mul(size, init_v2f(t, 1.0f)), pb->bar_color);
	rt_snprintf(buf, 32, "%f%%", t * 100.0f);
	draw_rect(pb->text.img, g_v2f_zero,
		init_v2f(pb->text.img->width, pb->text.img->height), C_BLACK);
	draw_text(pb->text.img, buf, g_v2f_zero, C_WHITE);
	draw_frame(bg->img, pb->bg.img, get_sprite_position(bg, &pb->bg, pb->pos));
	draw_frame(bg->img, pb->text.img,
		get_sprite_position(bg, &pb->text, pb->pos));
}
