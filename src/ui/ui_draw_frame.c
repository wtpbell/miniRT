/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_draw_frame.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 14:53:34 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "MLX42/MLX42.h"
#include "libft.h"
#include "rt_math.h"
#include "ui.h"

static bool	is_outside_draw_bounds(mlx_image_t *dst, mlx_image_t *src, t_v2i draw_pos)
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

t_v2i	get_sprite_position(t_sprite *parent, t_sprite *child, t_v2i pos)
{
	return ((t_v2i){
		.x = (parent->img->width * parent->anchor.x)
		- (child->img->width * child->anchor.x) + pos.x,
		.y = (parent->img->height * parent->anchor.y)
		- (child->img->height * child->anchor.y) + pos.y
	});
}



// TODO: Allow for scaling/sampling (optional)
// TODO: Push the start and end values within the dst window
void	draw_frame(mlx_image_t *dst, mlx_image_t *src, t_v2i draw_pos)
{
	t_v2i		start;
	t_v2i		end;
	t_v2i		sample;
	uint32_t	*pixel;
	uint32_t	*sample_pixel;

	if (is_outside_draw_bounds(dst, src, draw_pos))
		return ;
	start = clamp_draw_frame(draw_pos, init_v2i(0, 0), init_v2i(dst->width, dst->height));
	end = clamp_draw_frame(draw_pos, init_v2i(src->width, src->height), init_v2i(dst->width, dst->height));
	while (start.y < end.y)
	{
		sample.y = src->height - ((draw_pos.y + src->height) - start.y);
		while (start.x < end.x)
		{
			sample.x = src->width - ((draw_pos.x + src->width) - start.x);
			pixel = ((uint32_t *)dst->pixels) + (start.y * dst->width + start.x);
			sample_pixel = ((uint32_t *)src->pixels) + (sample.y * src->width + sample.x);
			mlx_put_pixel(dst, start.x, start.y, blend_colors(*pixel, *sample_pixel));
			++start.x;
		}
		start.x -= (sample.x + 1);
		++start.y;
	}
}
