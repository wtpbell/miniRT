/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   loading_screen.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/16 16:02:33 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 13:14:12 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

#define FRAME_COUNT 6
#define FPS			6.0f

void	destroy_load_screen(t_load_screen *load_screen, mlx_t *mlx)
{
	uint32_t	i;

	if (load_screen->bg.img != NULL)
		mlx_delete_image(mlx, load_screen->bg.img);
	i = 0;
	while (i < load_screen->ani.frame_count)
	{
		if (load_screen->ani.frames[i].img != NULL)
			mlx_delete_image(mlx, load_screen->ani.frames[i].img);
		++i;
	}
	free(load_screen->ani.frames);
	if (load_screen->pb.text.img != NULL)
		mlx_delete_image(mlx, load_screen->pb.text.img);
	if (load_screen->bg.img != NULL)
		mlx_delete_image(mlx, load_screen->pb.bg.img);
	free(load_screen);
}

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

t_load_screen	*init_load_screen(mlx_t *mlx)
{
	int				i;
	t_load_screen	*load_screen;

	load_screen = ft_calloc(1, sizeof(t_load_screen));
	load_screen->bg.anchor = init_v2f(0.5f, 0.5f);
	load_screen->bg.img = mlx_new_image(mlx, mlx->width, mlx->height);
	if (load_screen->bg.img == NULL
		|| mlx_image_to_window(mlx, load_screen->bg.img, 0, 0) == -1)
		return (destroy_load_screen(load_screen, mlx), NULL);
	load_screen->bg.img->enabled = false;

	load_screen->ani = (t_ani) {{{0, -64}}, 0, FRAME_COUNT, FPS, 0.0f, NULL};
	load_screen->ani.frames = ft_calloc(FRAME_COUNT, sizeof(t_sprite));
	if (load_screen->ani.frames == NULL)
		return (destroy_load_screen(load_screen, mlx), NULL);

	load_screen->ani.frames[0].img = init_str_frame(mlx, "LOADING   ");
	load_screen->ani.frames[1].img = init_str_frame(mlx, "LOADING   ");
	load_screen->ani.frames[2].img = init_str_frame(mlx, "LOADING.  ");
	load_screen->ani.frames[3].img = init_str_frame(mlx, "LOADING.. ");
	load_screen->ani.frames[4].img = init_str_frame(mlx, "LOADING...");
	load_screen->ani.frames[5].img = init_str_frame(mlx, "LOADING...");

	i = 0;
	while (i < FRAME_COUNT)
	{
		if (load_screen->ani.frames[i].img == NULL)
			return (destroy_load_screen(load_screen, mlx), NULL);
		load_screen->ani.frames[i].anchor = init_v2f(0.5f, 0.5f);
		++i;
	}

	if (!init_progress_bar(&load_screen->pb, (t_v2i){{0,0}}, (t_v2i){{512, 32}}, mlx))
		return (destroy_load_screen(load_screen, mlx), NULL);

	return (load_screen);
}

void	update_progress_bar(t_progress_bar *pb, t_sprite *bg, float t)
{
	const t_v2f	pos = init_v2f(pb->pos.x, pb->pos.y);
	const t_v2f	size = init_v2f(pb->size.x, pb->size.y);
	char		buf[16];

	draw_rect(pb->bg.img, pos, size, pb->bg_color);
	draw_rect(pb->bg.img, pos, v2f_mul(size, init_v2f(t, 1.0f)), pb->bar_color);
	rt_snprintf(buf, 32, "%f%%", t * 100.0f);
	
	draw_rect(pb->text.img, g_v2f_zero, init_v2f(pb->text.img->width, pb->text.img->height), C_BLACK);
	draw_text(pb->text.img, buf, g_v2f_zero, C_WHITE);

	draw_frame(bg->img, pb->bg.img, get_sprite_position(bg, &pb->bg, pb->pos));
	draw_frame(bg->img, pb->text.img, get_sprite_position(bg, &pb->text, pb->pos));
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

void	update_load_screen(t_load_screen *screen, float delta, float progress)
{
	if (!screen->bg.img->enabled)
		return ;
	ft_bzero(screen->bg.img->pixels,
			screen->bg.img->height * screen->bg.img->width * 4);
	update_animation(&screen->ani, &screen->bg, delta);
	update_progress_bar(&screen->pb, &screen->bg, progress);
}
