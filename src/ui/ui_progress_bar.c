/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_progress_bar.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/17 21:24:42 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 23:04:20 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

bool	init_progress_bar(t_progress_bar *pb, t_v2i pos, t_v2i size, mlx_t *mlx)
{
	pb->pos = pos;
	pb->size = size;
	pb->text.scale = g_v2f_one;
	pb->text.anchor = init_v2f(0.5f, 0.5f);
	pb->text.img = init_str_frame(mlx, "100.00%");
	if (pb->text.img == NULL)
		return (false);
	pb->bg.scale = g_v2f_one;
	pb->bg.anchor = init_v2f(0.5f, 0.5f);
	pb->bg.img = mlx_new_image(mlx, size.x, size.y);
	if (pb->bg.img == NULL)
		return (mlx_delete_image(mlx, pb->text.img), false);
	pb->bg_color = C_BLACK;
	pb->bar_color = C_RED;
	return (true);
}

void	update_progress_bar(t_progress_bar *pb, t_sprite *screen, float t)
{
	const t_v2f	pos = init_v2f(pb->pos.x, pb->pos.y);
	const t_v2f	size = init_v2f(pb->size.x, pb->size.y);
	char		buf[16];

	draw_rect(pb->bg.img, pos, size, pb->bg_color);
	draw_rect(pb->bg.img, pos, v2f_mul(size, init_v2f(t, 1.0f)), pb->bar_color);
	rt_snprintf(buf, 32, "%f%%", t * 100.0f);
	ft_bzero(pb->text.img->pixels,
		pb->text.img->width * pb->text.img->height * sizeof(uint32_t));
	draw_text(pb->text.img, buf, g_v2f_zero, C_WHITE);
	
	draw_frame(screen, &pb->bg, get_sprite_position(screen, &pb->bg, pb->pos));
	draw_frame(screen, &pb->text, get_sprite_position(screen, &pb->text, pb->pos));
}
