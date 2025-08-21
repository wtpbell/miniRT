/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:32:30 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:32:30 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

const t_perlin_node		g_nodes[4] = {
{"PINK", pink_noise},
{"TURB", turbulence_noise},
{"WOOD", wood_noise},
{"MARBLE", marble_noise}
};

void	init_params(t_pdisplay *display)
{
	const t_param	params[PARAMS_COUNT] = {
	{"DELTA X", {.f = &display->fdelta.x}, init_v2f(-P_POW, P_POW), NULL, NULL},
	{"DELTA Y", {.f = &display->fdelta.y}, init_v2f(-P_POW, P_POW), NULL, NULL},
	{"UNI SCALE", {.f = &display->offset.z}, init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"OFFSET X", {.f = &display->offset.x}, init_v2f(-P_POW, P_POW), NULL, NULL},
	{"OFFSET Y", {.f = &display->offset.y}, init_v2f(-P_POW, P_POW), NULL, NULL},
	{"RATE", {.f = &display->p_data->rate}, init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"GAIN", {.f = &display->p_data->gain}, init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"FREQ", {.f = &display->p_data->freq}, init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"AMPT", {.f = &display->p_data->ampt}, init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"LAYERS", {.i = &display->p_data->layers}, init_v2f(1.0f, 5.0f), NULL, NULL},
	{"DISTORTION", {.f = &display->p_data->marble.distortion},
		init_v2f(P_IPOW, P_POW), NULL, NULL},
	{"SCALE", {.f = &display->p_data->marble.scale}, init_v2f(P_IPOW, P_POW), NULL, NULL}
	};

	ft_memcpy(display->params, params, sizeof(params));
	display->param_count = PARAMS_COUNT;
	display->curr = 0;
}

bool	setup_perlin_ui(t_ui *ui, t_pdisplay *display)
{
	t_ui_element	*panel;
	t_ui_element	*section;
	t_v2f			size;
	char			header_text[128];

	size = init_v2f(UI_PANEL_WIDTH, UI_ROW_HEIGHT * (PARAMS_COUNT + 1));
	display->pattern = g_nodes[0];
	panel = create_panel(ui->context, g_v2f_zero, size);
	if (!panel)
		return (false);
	attach_child(ui->root, panel);
	rt_snprintf(header_text, sizeof(header_text),
		"PERLIN NOISE: %s", g_nodes[0].name);
	section = create_header(ui->context, header_text,
			g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT));
	if (!section)
		return (false);
	attach_child(panel, section);
	display->header = section;
	display->ui_panel = panel;
	if (!add_parameter_controls(ui, panel, display))
		return (false);
	ui_mark_dirty(ui->context);
	return (true);
}

bool	init_display(mlx_t *mlx, t_pdisplay *display, t_perlin *data)
{
	ft_memset(display, 0, sizeof(t_pdisplay));
	display->mlx = mlx;
	display->p_data = data;
	display->img = mlx_new_image(mlx, 512, 512);
	if (!display->img)
		return (false);
	display->offset = init_v3f(0.0f, 0.0f, 16.0f);
	display->curr = 0;
	display->fdelta = init_v2f(1.0f, -1.0f);
	display->idelta = (t_v2i){{1, -1}};
	display->pattern = (t_perlin_node){"pink", pink_noise};
	display->size = 24;
	display->values = init_modifiers(display, data);
	if (!display->values)
		return (false);
	init_params(display);
	display->ui = create_ui(mlx, NULL, NULL, display);
	if (!display->ui || !setup_perlin_ui(display->ui, display))
		return (false);
	if (mlx_image_to_window(mlx, display->img,
			(WIDTH - display->img->width) / 2,
			(HEIGHT - display->img->height) / 2) != 0)
		return (false);
	ui_mark_dirty(display->ui->context);
	return (true);
}

void	init_perlin_data(t_perlin *data)
{
	*data = (t_perlin){
		2.0f,
		0.5f,
		1.0f,
		1.0f,
		5,
	{1.0f, 1.0f}
	};
}
