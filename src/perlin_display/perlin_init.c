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

static const t_val_mod	g_valmods[] = {
{"delta-x", {.f = &((t_pdisplay *)0)->fdelta.x}, {.f = NULL}, double_flt},
{"delta-x", {.f = &((t_pdisplay *)0)->fdelta.x}, {.f = NULL}, half_flt},
{"uni_scale", {.f = &((t_pdisplay *)0)->offset.z}, {.f = NULL}, double_flt},
{"uni_scale", {.f = &((t_pdisplay *)0)->offset.z}, {.f = NULL}, half_flt},
{"offset-x", {.f = &((t_pdisplay *)0)->offset.x}, {.f = NULL}, double_flt},
{"offset-x", {.f = &((t_pdisplay *)0)->offset.x}, {.f = NULL}, half_flt},
{"offset-y", {.f = &((t_pdisplay *)0)->offset.y}, {.f = NULL}, double_flt},
{"offset-y", {.f = &((t_pdisplay *)0)->offset.y}, {.f = NULL}, half_flt},
{"rate", {.f = &((t_perlin *)0)->rate}, {.f = NULL}, double_flt},
{"rate", {.f = &((t_perlin *)0)->rate}, {.f = NULL}, half_flt},
{"gain", {.f = &((t_perlin *)0)->gain}, {.f = NULL}, double_flt},
{"gain", {.f = &((t_perlin *)0)->gain}, {.f = NULL}, half_flt},
{"freq", {.f = &((t_perlin *)0)->freq}, {.f = NULL}, double_flt},
{"freq", {.f = &((t_perlin *)0)->freq}, {.f = NULL}, half_flt},
{"ampt", {.f = &((t_perlin *)0)->ampt}, {.f = NULL}, double_flt},
{"ampt", {.f = &((t_perlin *)0)->ampt}, {.f = NULL}, half_flt},
{"layers", {.i = &((t_perlin *)0)->layers},
{.i = &((t_pdisplay *)0)->idelta.x}, delta_int},
{"layers", {.i = &((t_perlin *)0)->layers},
{.i = &((t_pdisplay *)0)->idelta.y}, delta_int},
{"dist", {.f = &((t_perlin *)0)->marble.distortion}, {.f = NULL}, double_flt},
{"dist", {.f = &((t_perlin *)0)->marble.distortion}, {.f = NULL}, half_flt},
{"scale", {.f = &((t_perlin *)0)->marble.scale}, {.f = NULL}, double_flt},
{"scale", {.f = &((t_perlin *)0)->marble.scale}, {.f = NULL}, half_flt}
};

bool	init_ui(t_pdisplay *display, t_perlin *data)
{
	display->size = sizeof(g_valmods) / sizeof(t_val_mod);
	display->values = ft_memdup(g_valmods, sizeof(g_valmods));
	(void)data;
	return (display->values != NULL);
}

void	init_params(t_pdisplay *display)
{
	const struct s_params	params[PARAMS_COUNT] = {
	{"DELTA X", &display->fdelta.x, init_v2f(-100.0f, 100.0f)},
	{"DELTA Y", &display->fdelta.y, init_v2f(-100.0f, 100.0f)},
	{"UNI SCALE", &display->offset.z, init_v2f(0.1f, 100.0f)},
	{"OFFSET X", &display->offset.x, init_v2f(-100.0f, 100.0f)},
	{"OFFSET Y", &display->offset.y, init_v2f(-100.0f, 100.0f)},
	{"RATE", &display->p_data->rate, init_v2f(0.1f, 10.0f)},
	{"GAIN", &display->p_data->gain, init_v2f(0.1f, 10.0f)},
	{"FREQ", &display->p_data->freq, init_v2f(0.1f, 10.0f)},
	{"AMPT", &display->p_data->ampt, init_v2f(0.1f, 10.0f)},
	{"LAYERS", (float *)&display->p_data->layers, init_v2f(1, 10)},
	{"DISTORTION", &display->p_data->marble.distortion, init_v2f(0.1f, 10.0f)},
	{"SCALE", &display->p_data->marble.scale, init_v2f(0.1f, 10.0f)}
	};

	ft_memcpy(g_params, params, sizeof(params));
}

void	init_display(mlx_t *mlx, t_pdisplay *display, t_perlin *data)
{
	display->mlx = mlx;
	display->p_data = data;
	display->img = mlx_new_image(mlx, 512, 512);
	display->offset = init_v3f(0.0f, 0.0f, 16.0f);
	display->curr = 0;
	display->size = -1;
	display->values = NULL;
	display->fdelta = init_v2f(1.0f, -1.0f);
	display->idelta = (t_v2i){{1, -1}};
	display->pattern = (t_perlin_node){"pink", pink_noise};
	display->ui = NULL;
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
