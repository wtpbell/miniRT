/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_modifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:08:15 by bewong            #+#    #+#             */
/*   Updated: 2025/08/20 13:08:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

static void	init_delta_scale_offset_modifiers(t_val_mod *mods,
	t_pdisplay *display, t_perlin *data)
{
	(void)data;
	mods[0] = (t_val_mod){"delta-x", {.f = &display->fdelta.x},
	{.f = NULL}, double_flt};
	mods[1] = (t_val_mod){"delta-x", {.f = &display->fdelta.x},
	{.f = NULL}, half_flt};
	mods[2] = (t_val_mod){"delta-y", {.f = &display->fdelta.y},
	{.f = NULL}, double_flt};
	mods[3] = (t_val_mod){"delta-y", {.f = &display->fdelta.y},
	{.f = NULL}, half_flt};
	mods[4] = (t_val_mod){"uni_scale", {.f = &display->offset.z},
	{.f = &display->fdelta.x}, delta_flt};
	mods[5] = (t_val_mod){"uni_scale", {.f = &display->offset.z},
	{.f = &display->fdelta.y}, delta_flt};
	mods[6] = (t_val_mod){"offset-x", {.f = &display->offset.x},
	{.f = &display->fdelta.x}, delta_flt};
	mods[7] = (t_val_mod){"offset-x", {.f = &display->offset.x},
	{.f = &display->fdelta.y}, delta_flt};
	mods[8] = (t_val_mod){"offset-y", {.f = &display->offset.y},
	{.f = &display->fdelta.x}, delta_flt};
	mods[9] = (t_val_mod){"offset-y", {.f = &display->offset.y},
	{.f = &display->fdelta.y}, delta_flt};
}

static void	init_rate_gain_modifiers(t_val_mod *mods,
	t_pdisplay *display, t_perlin *data)
{
	(void)display;
	mods[0] = (t_val_mod){"rate", {.f = &data->rate},
	{.f = &display->fdelta.x}, delta_flt};
	mods[1] = (t_val_mod){"rate", {.f = &data->rate},
	{.f = &display->fdelta.y}, delta_flt};
	mods[2] = (t_val_mod){"gain", {.f = &data->gain},
	{.f = &display->fdelta.x}, delta_flt};
	mods[3] = (t_val_mod){"gain", {.f = &data->gain},
	{.f = &display->fdelta.y}, delta_flt};
}

static void	init_freq_ampt_modifiers(t_val_mod *mods,
	t_pdisplay *display, t_perlin *data)
{
	(void)display;
	mods[0] = (t_val_mod){"freq", {.f = &data->freq},
	{.f = &display->fdelta.x}, delta_flt};
	mods[1] = (t_val_mod){"freq", {.f = &data->freq},
	{.f = &display->fdelta.y}, delta_flt};
	mods[2] = (t_val_mod){"ampt", {.f = &data->ampt},
	{.f = &display->fdelta.x}, delta_flt};
	mods[3] = (t_val_mod){"ampt", {.f = &data->ampt},
	{.f = &display->fdelta.y}, delta_flt};
}

static void	init_layers_marble_modifiers(t_val_mod *mods,
	t_pdisplay *display, t_perlin *data)
{
	mods[0] = (t_val_mod){"layers", {.i = &data->layers},
	{.i = &display->idelta.x}, delta_int};
	mods[1] = (t_val_mod){"layers", {.i = &data->layers},
	{.i = &display->idelta.y}, delta_int};
	if (data->layers < 1)
		data->layers = 1;
	mods[2] = (t_val_mod){"dist", {.f = &data->marble.distortion},
	{.f = &display->fdelta.x}, delta_flt};
	mods[3] = (t_val_mod){"dist", {.f = &data->marble.distortion},
	{.f = &display->fdelta.y}, delta_flt};
	mods[4] = (t_val_mod){"scale", {.f = &data->marble.scale},
	{.f = &display->fdelta.x}, delta_flt};
	mods[5] = (t_val_mod){"scale", {.f = &data->marble.scale},
	{.f = &display->fdelta.y}, delta_flt};
}

static const t_modifier	g_modifier_initializers[] = {
	init_delta_scale_offset_modifiers,
	init_rate_gain_modifiers,
	init_freq_ampt_modifiers,
	init_layers_marble_modifiers,
	NULL
};

t_val_mod	*init_modifiers(t_pdisplay *display, t_perlin *data)
{
	t_val_mod	*mods;
	t_val_mod	*current;
	int			i;

	mods = ft_calloc(24, sizeof(t_val_mod));
	if (!mods)
		return (NULL);
	current = mods;
	i = 0;
	while (g_modifier_initializers[i])
	{
		g_modifier_initializers[i](current, display, data);
		if (g_modifier_initializers[i] == init_delta_scale_offset_modifiers)
			current += 10;
		else if (g_modifier_initializers[i] == init_layers_marble_modifiers)
			current += 6;
		else
			current += 4;
		i++;
	}
	return (mods);
}
