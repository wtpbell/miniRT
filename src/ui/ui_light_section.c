/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_light_section.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:33:52 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 18:33:52 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	add_light_pos_controls(t_ui_context *ctx, t_light *light,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"POS X", "POS Y", "POS Z"};
	float			*values[3];
	int				i;

	i = 0;
	values[0] = &light->pos.x;
	values[1] = &light->pos.y;
	values[2] = &light->pos.z;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = values[i];
		cfg.range = init_v2f(-1000.0f, 1000.0f);
		cfg.step = 0.1f;
		cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + UI_PADDING + i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(&cfg, labels[i], size.x - (UI_PADDING * 2)));
		i++;
	}
}

static void	add_light_color_controls(t_ui_context *ctx, t_light *light,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"COL R", "COL G", "COL B"};
	float			*values[3];
	int				i;

	values[0] = &light->color.x;
	values[1] = &light->color.y;
	values[2] = &light->color.z;
	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = values[i];
		cfg.range = init_v2f(0.0f, 1.0f);
		cfg.step = 1.0f / 255.0f;
		cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + 3 * (UI_ROW_HEIGHT + UI_PADDING) + 
			i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_color_value;
		attach_child(section, create_labeled_control(&cfg, labels[i], size.x - (UI_PADDING * 2)));
		i++;
	}
}

static void	add_light_intensity_control(t_ui_context *ctx, t_light *light,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &light->intensity;
	cfg.range = init_v2f(0.0f, 10.0f);
	cfg.step = 0.1f;
	cfg.pos = init_v2f(UI_PADDING,
		UI_HEADER_HEIGHT + 6 * (UI_ROW_HEIGHT + UI_PADDING) + UI_PADDING);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "INTENSITY", size.x - (UI_PADDING * 2)));
}

t_ui_element	*create_light_section(t_ui_context *ctx, t_scene *scene,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	t_light			*light;

	light = find_light(scene, LIGHT_POINT);
	if (!light)
		return (NULL);
	size.y = UI_HEADER_HEIGHT + 7 * (UI_ROW_HEIGHT + UI_PADDING) + UI_PADDING;
	section = create_panel(ctx, pos, size);
	if (!section)
		return (NULL);

	attach_child(section, create_header(ctx, "LIGHT",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT)));

	add_light_pos_controls(ctx, light, section, size);
	add_light_color_controls(ctx, light, section, size);
	add_light_intensity_control(ctx, light, section, size);

	return (section);
}
