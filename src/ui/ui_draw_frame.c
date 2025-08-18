/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw_frame.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 23:13:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "MLX42/MLX42.h"
#include "MLX42/MLX42_Int.h"
#include "libft.h"
#include "rt_math.h"
#include "ui.h"

t_v2i	get_sprite_position(t_sprite *parent, t_sprite *child, t_v2i pos)
{
	return ((t_v2i){
		.x = (parent->img->width * parent->anchor.x)
		- (child->img->width * child->anchor.x) + pos.x,
		.y = (parent->img->height * parent->anchor.y)
		- (child->img->height * child->anchor.y) + pos.y
	});
}

/* Based on mlx_put_string but does not add image to window */
mlx_image_t	*init_str_frame(mlx_t *mlx, const char *str)
{
	mlx_image_t		*strimage;
	const size_t	len = strlen(str);

	if (len > MLX_MAX_STRING)
		return ((void *)mlx_error(MLX_STRTOOBIG));
	strimage = mlx_new_image(mlx, len * UI_CHAR_WIDTH, UI_FONT_HEIGHT);
	if (strimage == NULL)
		return (NULL);
	draw_text(strimage, str, init_v2f(0, 0), C_WHITE);
	return (strimage);
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
	draw_frame(bg->img, frame->img,
		get_sprite_position(bg, frame, ani->pos));
}

// TODO: Allow for scaling/sampling (optional)
// TODO: Push the start and end values within the dst window
void	draw_frame(mlx_image_t *dst, mlx_image_t *src, t_v2i draw_pos)
{
	t_v2i				start;
	t_v2i				end;
	t_v2i				sample;
	uint32_t			*pixel;
	uint32_t			*sample_pixel;

	if (draw_pos.x >= (int)dst->width || draw_pos.y >= (int)dst->height
		|| draw_pos.x + (int)src->width < 0 || draw_pos.y + (int)src->height < 0)
		return ;

	sample = (t_v2i){{0, 0}};
	start = draw_pos;
	end.x = start.x + src->width;
	end.y = start.y + src->height;
	while (start.y < end.y)
	{	
		if (start.y >= 0 && start.y < (int)dst->height)
		{
			sample.y = src->height - (end.y - start.y);
			while (start.x < end.x)
			{
				if (start.x >= 0 && start.x < (int)dst->width)
				{
					sample.x = src->width - (end.x - start.x);
					pixel = ((uint32_t *)dst->pixels) + (start.y * dst->width + start.x);
					sample_pixel = ((uint32_t *)src->pixels) + (sample.y * src->width + sample.x);
					*pixel = blend_colors(*pixel, *sample_pixel);
				}
				++start.x;
			}
		}
		start.x = draw_pos.x;
		++start.y;
	}
}
