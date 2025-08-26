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
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = &light->pos._axis[i];
		cfg.range = init_v2f(MIN_POS, MAX_POS);
		cfg.step = 0.1f;
		cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + i
				* (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(&cfg, labels[i],
				size.x - (2 * UI_PADDING)));
		i++;
	}
}

static void	add_light_color_controls(t_ui_context *ctx, t_light *light,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"COL R", "COL G", "COL B"};
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = &light->color._axis[i];
		cfg.range = init_v2f(0.0f, 1.0f);
		cfg.step = 10.0f / 255.0f;
		cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT
				+ 3.2 * (UI_ROW_HEIGHT + UI_PADDING)
				+ i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_color_value;
		attach_child(section, create_labeled_control(&cfg, labels[i],
				size.x - (2 * UI_PADDING)));
		i++;
	}
}

static void	add_light_intensity_control(t_ui_context *ctx, t_light *light,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &light->intensity;
	cfg.range = init_v2f(MIN_LIGHT_RATIO, MAX_LIGHT_RATIO);
	cfg.step = 0.1f;
	cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + 6.1
			* (UI_ROW_HEIGHT + UI_PADDING));
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "INTENSITY",
			size.x - (2 * UI_PADDING)));
}

t_ui_element	*create_light_section(t_section_config *cfg)
{
	t_ui_element	*section;
	t_light			*light;
	t_v2f			size;

	if (!cfg || !cfg->ctx || !cfg->sample || !cfg->ctx->scene)
		return (NULL);
	light = find_light(cfg->ctx->scene, LIGHT_POINT);
	if (!light)
		return (NULL);
	size = init_v2f(cfg->size.x,
			UI_HEADER_HEIGHT + 7.1 * (UI_ROW_HEIGHT + UI_PADDING) + UI_PADDING);
	section = create_panel(cfg->ctx, cfg->pos, size);
	if (!section)
		return (NULL);
	section->style.bg_color = UI_COLOR_LIGHT_SECTION;
	attach_child(section, create_header(cfg->ctx, "LIGHT",
			g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT)));
	add_light_pos_controls(cfg->ctx, light, section, size);
	add_light_color_controls(cfg->ctx, light, section, size);
	add_light_intensity_control(cfg->ctx, light, section, size);
	return (section);
}
