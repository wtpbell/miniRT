/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   animate.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 14:50:52 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "MLX42/MLX42.h"
#include "libft.h"
#include "rt_math.h"

typedef struct s_frame
{
	mlx_image_t	*sprite;
}	t_frame;

typedef struct s_animation
{
	t_v2i		pos;
	uint32_t	idx;
	uint32_t	frame_count;
	float		fps;
	float		time;
	t_frame		*frames;
}	t_ani;

typedef struct s_load_screen
{
	mlx_image_t	*background;
	t_ani		*ani;

}	t_load_screen;



t_frame	*init_str_frame(const char *str)
{
	// mlx_put_string
	return (NULL);
}

// position can be out of bounds; it will need to offset the starting pos if any part of the sprite would be still drawn
void	update_load_screen(t_load_screen *screen, float delta)
{
	t_ani		*ani;
	t_frame		*frame;
	uint32_t	next_idx;
	uint32_t	row;
	uint32_t	copy_width;

	ani = screen->ani;
	ani->time += delta * ani->fps;
	next_idx = (int)ani->time % ani->frame_count;
	if (next_idx != ani->idx)
	{
		ft_bzero(screen->background, screen->background->width * screen->background->height);

		row = 0;
		frame = ani->frames + next_idx;
		if (ani->pos.x >= screen->background->width)
			return ;
		if (screen->background->width - ani->pos.x > frame->sprite->width)
			copy_width = frame->sprite->width;
		else
			copy_width = screen->background->width - ani->pos.x;

		while (row < frame->sprite->height && ani->pos.y + row < screen->background->height)
		{
			ft_memcpy(
				screen->background->pixels + (ani->pos.y + row) * screen->background->width,
				frame->sprite->pixels + row * frame->sprite->width,
				copy_width);
		}

		ani->idx = next_idx;
	}
}
