/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_ambient_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:59:58 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 17:59:58 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	add_ambient_color_controls(t_ui_context *ctx, t_light *ambient,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"COLOR R", "COLOR G", "COLOR B"};
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = &ambient->color._axis[i];
		cfg.range = init_v2f(0, 1.0f);
		cfg.step = 10.0f / 255.0f;
		cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + i
				* (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_color_value;
		attach_child(section, create_labeled_control(&cfg, labels[i],
				size.x - (UI_PADDING * 2)));
		i++;
	}
}

static void	add_ambient_intensity_control(t_ui_context *ctx, t_light *ambient,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &ambient->intensity;
	cfg.range = init_v2f(0, 1);
	cfg.step = 0.1f;
	cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + UI_PADDING + 3 * (UI_ROW_HEIGHT + UI_PADDING));
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "INTENSITY",
			size.x - (UI_PADDING * 2)));
}

t_ui_element	*create_ambient_section(t_section_config *cfg)
{
	t_ui_element	*section;
	t_light			*ambient;
	t_v2f			size;

	ambient = find_light(cfg->ctx->scene, LIGHT_AMBIENT);
	if (!ambient)
		return (NULL);
	size = init_v2f(cfg->size.x,
			UI_HEADER_HEIGHT + 4 * (UI_ROW_HEIGHT + UI_PADDING) + UI_PADDING);
	section = create_panel(cfg->ctx, cfg->pos, size);
	if (!section)
		return (NULL);
	section->style.bg_color = UI_COLOR_AMBIENT_SECTION;
	attach_child(section, create_header(cfg->ctx, "AMBIENT LIGHT",
			g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT)));
	add_ambient_color_controls(cfg->ctx, ambient, section, size);
	add_ambient_intensity_control(cfg->ctx, ambient, section, size);
	return (section);
}
