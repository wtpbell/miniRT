/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   animate.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/14 17:28:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/15 19:10:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include "MLX42/MLX42.h"
#include "MLX42/MLX42_Int.h"
#include "libft.h"
#include "rt_math.h"
#include "ui.h"

#define FRAME_COUNT 6

/* Based on mlx_put_string but does not add image to window */
mlx_image_t	*init_str_frame(mlx_t* mlx, const char *str)
{
	mlx_image_t*	strimage;
	const size_t	len = strlen(str);

	if (len > MLX_MAX_STRING)
		return ((void*)mlx_error(MLX_STRTOOBIG));
	if (!(strimage = mlx_new_image(mlx, len * UI_CHAR_WIDTH, UI_FONT_HEIGHT)))
		return (NULL);
	draw_text(strimage, str, init_v2f(0,0), C_WHITE);
	return (strimage);
}

void	destroy_load_screen(t_load_screen *load_screen, mlx_t *mlx)
{
	uint32_t	i;

	if (load_screen->background != NULL)
		mlx_delete_image(mlx, load_screen->background);
	i = 0;
	while (i < load_screen->ani.frame_count)
	{
		if (load_screen->ani.frames[i].sprite != NULL)
			mlx_delete_image(mlx, load_screen->ani.frames[i].sprite);
		++i;
	}
	free(load_screen->ani.frames);
	free(load_screen);
}

t_load_screen	*init_load_screen(mlx_t *mlx)
{
	int				i;
	t_load_screen	*load_screen;

	load_screen = ft_calloc(1, sizeof(t_load_screen));
	load_screen->background = mlx_new_image(mlx, mlx->width, mlx->height);
	if (load_screen->background == NULL
		|| mlx_image_to_window(mlx, load_screen->background, 0, 0) == -1)
		return (destroy_load_screen(load_screen, mlx), NULL);
	load_screen->background->enabled = false;
	load_screen->ani = (t_ani) {{{0, 0}}, 0, FRAME_COUNT, 4.0f, 0.0f, NULL};
	load_screen->ani.frames = ft_calloc(FRAME_COUNT, sizeof(t_frame));
	if (load_screen->ani.frames == NULL)
		return (destroy_load_screen(load_screen, mlx), NULL);

	load_screen->ani.pos = (t_v2i){{500, 500}};
	load_screen->ani.frames[0].sprite = init_str_frame(mlx, "LOADING   ");
	load_screen->ani.frames[1].sprite = init_str_frame(mlx, "LOADING   ");
	load_screen->ani.frames[2].sprite = init_str_frame(mlx, "LOADING.  ");
	load_screen->ani.frames[3].sprite = init_str_frame(mlx, "LOADING.. ");
	load_screen->ani.frames[4].sprite = init_str_frame(mlx, "LOADING...");
	load_screen->ani.frames[5].sprite = init_str_frame(mlx, "LOADING...");

	i = 0;
	while (i < FRAME_COUNT)
	{
		if (load_screen->ani.frames[i].sprite == NULL)
			return (destroy_load_screen(load_screen, mlx), NULL);
		++i;
	}
	return (load_screen);
}

void	draw_frame(t_ani *ani, mlx_image_t *canvas)
{
	const mlx_image_t	*frame = ani->frames[ani->idx].sprite;
	t_v2i				start;
	t_v2i				end;
	t_v2i				offset;
	uint32_t			copy_width;
	uint32_t			copy_height;
	uint32_t			*pixel;

	// is frame within canvas?
	if (ani->pos.x > canvas->width || ani->pos.y > canvas->height
		|| ani->pos.x + frame->width < 0 || ani->pos.y + frame->height < 0)
		return ;

	// adjust starting drawing position and maximum width and height
	if (ani->pos.x < 0)
	{
		start.x = 0;
		copy_width = ani->pos.x + frame->width;
	}
	else if (ani->pos.x + frame->width > canvas->width)
		copy_width = canvas->width - ani->pos.x;
	else
		copy_width = frame->width;

	if (ani->pos.y < 0)
	{
		start.y = 0;
		copy_height = ani->pos.y + frame->height;
	}
	else if (ani->pos.y + frame->height > canvas->height)
		copy_height = canvas->height - ani->pos.y;
	else
		copy_height = frame->height;

	end.x = start.x + copy_width;
	end.y = start.y + copy_height;
	while (start.y < end.y)
	{
		while (start.x < end.x)
		{
			pixel = (uint32_t *)canvas->pixels[start.y * canvas->width + start.x];
			pixel = blend_colors(pixel, frame->pixels[0]);
			++start.x;
		}
		++start.y;
	}

}

// position can be out of bounds; it will need to offset the starting pos if any part of the sprite would be still drawn
void	update_load_screen(t_load_screen *screen, float delta)
{
	t_ani		*ani;
	t_frame		*frame;
	uint32_t	next_idx;
	uint32_t	row;
	uint32_t	copy_width;

	if (!screen->background->enabled)
		return ;
	ani = &screen->ani;
	ani->time += delta * ani->fps;
	next_idx = (int)ani->time % ani->frame_count;
	if (next_idx != ani->idx)
	{
		ft_bzero(screen->background->pixels,
			screen->background->height * screen->background->width * 4);

		row = 0;
		frame = ani->frames + next_idx;
		if (ani->pos.x >= (int)screen->background->width)
			return ;
		if ((int)screen->background->width - ani->pos.x > (int)frame->sprite->width)
			copy_width = frame->sprite->width;
		else
			copy_width = screen->background->width - ani->pos.x;

		while (row < frame->sprite->height && ani->pos.y + row < screen->background->height)
		{
			// This doesn't respect the background color
			ft_memcpy(
				screen->background->pixels + (ani->pos.y + row) * screen->background->width * 4,
				frame->sprite->pixels + row * frame->sprite->width * 4,
				copy_width * 4);
			++row;
		}
		ani->idx = next_idx;
	}
}
