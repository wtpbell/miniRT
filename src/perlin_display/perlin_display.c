/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_display.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 08:59:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/24 15:11:11 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"
#include "rt_error.h"

static void	perlin_close_window(void *param)
{
	t_pdisplay	*display;

	display = (t_pdisplay *)param;
	if (display->exit_state != PD_ERR)
		display->exit_state = PD_QUIT;
}

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
		return (cleanup_display(d), false);
	return (true);
}

int	perlin_display(void)
{
	mlx_t		*mlx;
	t_pdisplay	display;
	t_perlin	data;

	ft_bzero(&display, sizeof(t_pdisplay));
	init_perlin_data(&data);
	if (!init_window_and_display(&mlx, &display, &data))
		return (rt_exit());
	mlx_close_hook(mlx, perlin_close_window, &display);
	mlx_key_hook(mlx, perlin_key_hook, &display);
	if (mlx_loop_hook(mlx, perlin_render_loop, display.ui))
		mlx_loop(mlx);
	cleanup_display(&display);
	if (display.exit_state == PD_QUIT)
		errno = 0;
	return (rt_exit());
}
