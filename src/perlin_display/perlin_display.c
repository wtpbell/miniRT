/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:59:47 by jboon             #+#    #+#             */
/*   Updated: 2025/08/17 16:08:47 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

struct s_params		g_params[PARAMS_COUNT];

const t_perlin_node	g_nodes[4] = {
{"PINK", pink_noise},
{"TURB", turbulence_noise},
{"WOOD", wood_noise},
{"MARBLE", marble_noise}
};

void	setup_perlin_ui(t_ui *ui, t_pdisplay *display)
{
	t_ui_element	*panel;
	t_ui_element	*section;
	t_v2f			size;
	char			header_text[128];

	size = init_v2f(UI_PANEL_WIDTH, UI_ROW_HEIGHT * (PARAMS_COUNT + 1));
	display->pattern = g_nodes[0];
	draw_perlin(display->img, display->p_data,
		display->offset, g_nodes[0].fp_perlin);
	panel = create_panel(ui->context, g_v2f_zero, size);
	attach_child(ui->root, panel);
	rt_snprintf(header_text, sizeof(header_text),
		"PERLIN NOISE: %s", g_nodes[0].name);
	section = create_header(ui->context, header_text,
			g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT));
	attach_child(panel, section);
	display->header = section;
	display->ui_panel = panel;
	add_parameter_controls(ui, panel, display);
	ui_mark_dirty(ui->context);
}

static void	perlin_render_loop(void *param)
{
	t_ui	*ui;

	ui = (t_ui *)param;
	ft_memset(ui->context->canvas->pixels, 0,
		ui->context->canvas->width * ui->context->canvas->height
		* sizeof(uint32_t));
	render_ui_element(ui->root, ui->context);
	ui->context->needs_redraw = false;
}

static bool	init_window_and_display(mlx_t **mlx, t_pdisplay *d, t_perlin *p)
{
	init_perlin();
	*mlx = mlx_init(WIDTH, HEIGHT, "Perlin Noise Display", false);
	if (!*mlx)
		return (false);
	init_display(*mlx, d, p);
	init_params(d);
	if (!d->img)
	{
		perror("perlin_display:display.img");
		mlx_terminate(*mlx);
		return (false);
	}
	return (true);
}

static void	setup_ui_and_first_frame(mlx_t *mlx, t_pdisplay *d, t_perlin *p)
{
	d->ui = create_ui(mlx, NULL, NULL, d);
	if (!d->ui)
		perror("perlin_display");
	else
		setup_perlin_ui(d->ui, d);
	draw_perlin(d->img, p, d->offset, d->pattern.fp_perlin);
	mlx_image_to_window(mlx, d->img,
		(WIDTH - d->img->width) / 2,
		(HEIGHT - d->img->height) / 2);
}

void	perlin_display(void)
{
	mlx_t		*mlx;
	t_pdisplay	display;
	t_perlin	data;

	init_perlin_data(&data);
	if (!init_window_and_display(&mlx, &display, &data))
		return ;
	setup_ui_and_first_frame(mlx, &display, &data);
	mlx_key_hook(mlx, perlin_key_hook, &display);
	if (display.ui)
		mlx_loop_hook(mlx, perlin_render_loop, display.ui);
	mlx_loop(mlx);
	if (display.ui)
		destroy_ui(display.ui);
	free(display.values);
	mlx_terminate(mlx);
}
