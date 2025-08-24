/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_display.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 08:59:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/23 17:29:12 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

static void	perlin_render_loop(void *param)
{
	t_ui		*ui;
	t_pdisplay	*display;

	ui = (t_ui *)param;
	display = (t_pdisplay *)ui->context->game;
	if (ui->context->needs_redraw)
	{
		draw_perlin(display->img, display->p_data,
			display->offset, display->pattern.fp_perlin);
		render_ui_element(ui->root, ui->context);
		ui->context->needs_redraw = false;
	}
}

static bool	init_window_and_display(mlx_t **mlx, t_pdisplay *d, t_perlin *p)
{
	init_perlin();
	*mlx = mlx_init(WIDTH, HEIGHT, "Perlin Noise Display", false);
	if (!*mlx)
		return (false);
	if (!init_display(*mlx, d, p))
		return (sys_error("init_window_and_display"), cleanup_display(d), false);
	return (true);
}

void	perlin_display(void)
{
	mlx_t		*mlx;
	t_pdisplay	display;
	t_perlin	data;

	ft_bzero(&display, sizeof(t_pdisplay));
	init_perlin_data(&data);
	if (!init_window_and_display(&mlx, &display, &data))
		return ;
	mlx_key_hook(mlx, perlin_key_hook, &display);
	mlx_loop_hook(mlx, perlin_render_loop, display.ui);
	mlx_loop(mlx);
	cleanup_display(&display);
}
