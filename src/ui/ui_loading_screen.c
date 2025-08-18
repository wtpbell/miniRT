/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_loading_screen.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/16 16:02:33 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/18 14:38:40 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "MLX42/MLX42_Int.h"

#define FRAME_COUNT 6
#define FPS			6.0f

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

static bool	init_animation(t_ani *animation, mlx_t *mlx, t_v2i pos,
	t_v2f anchor)
{
	const char	*str_frames[FRAME_COUNT] = {"LOADING   ", "LOADING   ",
		"LOADING.  ", "LOADING.. ", "LOADING...", "LOADING..."
	};
	int			i;
	t_sprite	*frames;

	*animation = (t_ani){pos, 0, FRAME_COUNT, FPS, 0.0f, NULL};
	animation->frames = ft_calloc(FRAME_COUNT, sizeof(t_sprite));
	if (animation->frames == NULL)
		return (false);
	i = 0;
	frames = animation->frames;
	while (i < FRAME_COUNT)
	{
		frames[i].img = init_str_frame(mlx, str_frames[i]);
		if (frames[i].img == NULL)
			return (false);
		frames[i].anchor = anchor;
		++i;
	}
	return (true);
}

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

t_load_screen	*init_load_screen(mlx_t *mlx)
{
	t_load_screen	*load_screen;

	load_screen = ft_calloc(1, sizeof(t_load_screen));
	if (load_screen == NULL)
		return (NULL);
	load_screen->bg.anchor = init_v2f(0.5f, 0.5f);
	load_screen->bg.img = mlx_new_image(mlx, mlx->width, mlx->height);
	if (load_screen->bg.img == NULL
		|| mlx_image_to_window(mlx, load_screen->bg.img, 0, 0) == -1)
		return (destroy_load_screen(load_screen, mlx), NULL);
	if (!init_animation(&load_screen->ani, mlx,
			(t_v2i){{0, -64}}, init_v2f(0.5f, 0.5f)))
		return (destroy_load_screen(load_screen, mlx), NULL);
	if (!init_progress_bar(&load_screen->pb,
			(t_v2i){{0, 0}}, (t_v2i){{512, 32}}, mlx))
		return (destroy_load_screen(load_screen, mlx), NULL);
	return (load_screen);
}

/**
 * For the ANSI escape printing:
 * https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
 */
void	update_load_screen(t_load_screen *screen, float delta, float progress)
{
	if (!screen->bg.img->enabled)
		return ;
	ft_bzero(screen->bg.img->pixels,
		screen->bg.img->height * screen->bg.img->width * sizeof(uint32_t));
	update_animation(&screen->ani, &screen->bg, delta);
	update_progress_bar(&screen->pb, &screen->bg, progress);
	printf("\x1b[1F\x1b[2K");
	printf("RENDERING: %.2f%%\n", progress * 100.0f);
}
