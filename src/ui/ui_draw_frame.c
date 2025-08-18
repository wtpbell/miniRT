/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_draw_frame.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 23:42:30 by jboon         ########   odam.nl         */
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

static inline uint32_t	*get_pixel(mlx_image_t *img, int offset)
{
	return (((uint32_t *)img->pixels) + offset);
}

// TODO: Allow for scaling/sampling (optional)
static uint32_t	sample_pixel(t_v2i pixel, t_v2f scale, mlx_image_t *src)
{
	const t_v2i	local_pixel = init_v2i(pixel.x / scale.x, pixel.y / scale.y);
	return (*get_pixel(src, local_pixel.y * src->width + local_pixel.x));
}

static inline void	set_pixel(mlx_image_t *dst, mlx_image_t *src, t_v2i pos,
	t_v2i smp, t_v2f scale)
{
	uint32_t	smp_pxl;

	smp_pxl = sample_pixel(smp, scale, src);// get_pixel(src, smp.y * src->width + smp.x);
	if (get_a(smp_pxl) == 0)
		return ;
	mlx_put_pixel(dst, pos.x, pos.y,
		blend_colors(*get_pixel(dst, pos.y * dst->width + pos.x), smp_pxl));
}

void	draw_frame(t_sprite *dst, t_sprite *src, t_v2i draw_pos)
{
	const t_v2i	sample_size = init_v2i(src->img->width * src->scale.x, src->img->height * src->scale.y);
	const t_v2i	canvas_size = init_v2i(dst->img->width, dst->img->height);
	t_v2i		start;
	t_v2i		end;
	t_v2i		smp;


	if (is_outside_draw_bounds(dst->img, src->img, draw_pos))
		return ;
	start = clamp_draw_frame(draw_pos, init_v2i(0, 0), canvas_size);
	end = clamp_draw_frame(draw_pos, sample_size, canvas_size);
	while (start.y < end.y)
	{
		smp.y = sample_size.y - ((draw_pos.y + sample_size.y) - start.y);
		while (start.x < end.x)
		{
			smp.x = sample_size.x - ((draw_pos.x + sample_size.x) - start.x);
			set_pixel(dst->img, src->img, start, smp, src->scale);
			++start.x;
		}
		start.x -= (smp.x + 1);
		++start.y;
	}
}


// void	draw_frame(mlx_image_t *dst, mlx_image_t *src, t_v2i draw_pos)
// {
// 	const t_v2i	size = init_v2i(src->width * 4, src->height * 4);
// 	t_v2i	start;
// 	t_v2i	end;
// 	t_v2i	smp;

// 	if (is_outside_draw_bounds(dst, src, draw_pos))
// 		return ;
// 	start = clamp_draw_frame(draw_pos, init_v2i(0, 0), init_v2i(dst->width, dst->height));
// 	end = clamp_draw_frame(draw_pos, size, init_v2i(dst->width, dst->height));
// 	while (start.y < end.y)
// 	{
// 		smp.y = src->height - ((draw_pos.y + src->height) - start.y);
// 		while (start.x < end.x)
// 		{
// 			smp.x = src->width - ((draw_pos.x + src->width) - start.x);
// 			set_pixel(dst, src, start, smp);
// 			++start.x;
// 		}
// 		start.x -= (smp.x + 1);
// 		++start.y;
// 	}
// }
