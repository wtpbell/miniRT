/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_draw_frame.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/20 21:34:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "MLX42/MLX42.h"
#include "libft.h"
#include "rt_math.h"
#include "ui.h"

static bool	is_outside_draw_bounds(mlx_image_t *dst, mlx_image_t *src,
	t_v2i draw_pos)
{
	return (draw_pos.x >= (int)dst->width
		|| draw_pos.y >= (int)dst->height
		|| draw_pos.x + (int)src->width < 0
		|| draw_pos.y + (int)src->height < 0);
}

static t_v2i	clamp_draw_frame(t_v2i draw_pos, t_v2i offset, t_v2i max)
{
	return ((t_v2i){
		.x = fmaxf(0.0f, fminf(draw_pos.x + offset.x, max.x)),
		.y = fmaxf(0.0f, fminf(draw_pos.y + offset.y, max.y))
	});
}

static void	put_pixel_frame(t_sprite *dst, t_sprite *src, t_v2i pos_dst,
	t_v2i pos_src)
{
	uint32_t	color;

	color = rt_get_pixel(src->img, pos_src.x / src->scale.x,
		pos_src.y / src->scale.y);
	if ((color & 0xFF) == 0)
		return ;
	mlx_put_pixel(dst->img, pos_dst.x, pos_dst.y,
		blend_colors(rt_get_pixel(dst->img, pos_dst.x, pos_dst.y), color));
}

void	draw_frame(t_sprite *dst, t_sprite *src, t_v2i pos)
{
	const t_v2i	max = init_v2i(pos.x + src->full_size.x,
		pos.y + src->full_size.y);
	t_v2i		start;
	t_v2i		end;
	t_v2i		smp;

	if (is_outside_draw_bounds(dst->img, src->img, pos))
		return ;
	smp = init_v2i(0, 0);
	start = clamp_draw_frame(pos, init_v2i(0, 0), dst->full_size);
	end = clamp_draw_frame(pos, src->full_size, dst->full_size);
	while (start.y < end.y)
	{
		smp.y = src->full_size.y - (max.y - start.y);
		while (start.x < end.x)
		{
			smp.x = src->full_size.x - (max.x - start.x);
			put_pixel_frame(dst, src, start, smp);
			++start.x;
		}
		start.x -= (smp.x + 1);
		++start.y;
	}
}
