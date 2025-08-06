/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 19:43:06 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

t_ui_element	*create_header(t_ui_context *ctx, const char *title,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_element	*title_label;

	header = create_panel(ctx, pos, size);
	if (!header)
		return (NULL);
	if (title)
	{
		title_label = create_label(ctx, title, init_v2f(10, 10), 0xFFFFFFFF);
		if (title_label)
		{
			title_label->pos = init_v2f(10, (size.y - UI_FONT_HEIGHT) / 2);
			attach_child(header, title_label);
		}
	}
	return (header);
}

static void	init_value_button_data(t_ui_value_button *value_btn, const t_vbtn_config *cfg)
{
	if (!value_btn || !cfg)
		return ;
	value_btn->value = cfg->value;
	value_btn->range = cfg->range;
	value_btn->step = cfg->step;
	value_btn->on_click = NULL;
	value_btn->param = NULL;
	value_btn->formatter = cfg->formatter;
}

static void	add_inc_dec_buttons(t_ui_element *container,
						const t_vbtn_config *cfg, t_ui_value_button *value_btn)
{
	t_ui_element	*decr_btn;
	t_ui_element	*incr_btn;

	(void)value_btn;
	decr_btn = create_button(cfg->ctx, "-",
		init_v2f(0, 0),
		init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		decrement_value_button, cfg->ctx);
	incr_btn = create_button(cfg->ctx, "+",
		init_v2f(cfg->size.x - UI_BUTTON_WIDTH, 0),
		init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		increment_value_button, cfg->ctx);
	if (decr_btn)
		attach_child(container, decr_btn);
	if (incr_btn)
		attach_child(container, incr_btn);
}

static void	add_value_label(t_ui_element *container,
	const t_vbtn_config *cfg, t_ui_value_button *value_btn)
{
	char		*value_str;
	float		label_width;
	float		label_x;
	t_ui_element	*label;

	if (cfg->formatter)
		value_str = ft_strdup(cfg->formatter(*cfg->value));
	else
		value_str = ft_ftoa(*cfg->value, 2);
	if (!value_str)
		return;
	label_width = ft_strlen(value_str) * UI_CHAR_WIDTH;
	label_x = (cfg->size.x - label_width) / 2.0f;
	label = create_label(cfg->ctx, value_str,
		init_v2f(label_x, (cfg->size.y - UI_FONT_HEIGHT) / 2),
		UI_TEXT_COLOR);
	if (label)
	{
		value_btn->value_label = label;
		attach_child(container, label);
	}
	free(value_str);
}

static t_ui_element	*create_value_button(t_vbtn_config *cfg)
{
	t_ui_element		*container;
	t_ui_value_button	*value_btn;

	container = create_panel(cfg->ctx, cfg->pos, cfg->size);
	if (!container)
		return (NULL);
	value_btn = (t_ui_value_button *)ft_calloc(1, sizeof(t_ui_value_button));
	if (!value_btn)
		return container;
	init_value_button_data(value_btn, cfg);
	add_inc_dec_buttons(container, cfg, value_btn);
	add_value_label(container, cfg, value_btn);
	container->type = UI_VALUE_BUTTON;
	container->data = value_btn;
	return container;
}

t_ui_element	*create_labeled_control(t_vbtn_config *cfg,
						const char *label_text, float total_width)
{
	float			label_width;
	float			control_width;
	float			control_x;
	t_ui_element	*container;
	t_ui_element	*label;

	container = create_panel(cfg->ctx, cfg->pos, init_v2f(total_width, UI_ROW_HEIGHT));
	if (!container)
		return (NULL);
	label_width = total_width * UI_LABEL_WIDTH_RATIO;
	control_width = total_width - label_width - UI_PADDING;
	control_x = label_width + UI_PADDING;
	label = create_label(cfg->ctx, label_text,
		init_v2f(UI_LABEL_PADDING, (UI_ROW_HEIGHT - UI_FONT_HEIGHT) / 2),
		UI_LABEL_COLOR);
	if (label)
		attach_child(container, label);
	cfg->pos = init_v2f(control_x, 0);
	cfg->size = init_v2f(control_width, UI_ROW_HEIGHT);
	attach_child(container, create_value_button(cfg));
	return (container);
}

t_ui_element	*create_ui_sections(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;
	t_v2f			section_size;
	t_v2f			section_pos;
	t_v3f			section_heights;

	section_heights = (t_v3f){
		.x = UI_HEADER_HEIGHT + 8 * (UI_ROW_HEIGHT + UI_PADDING),
		.y = UI_HEADER_HEIGHT + 8 * (UI_ROW_HEIGHT + UI_PADDING),
		.z = UI_HEADER_HEIGHT + 5 * (UI_ROW_HEIGHT + UI_PADDING)
	};
	panel = create_panel(ctx, pos, size);
	if (!panel)
		return (NULL);
	section_pos = init_v2f(UI_PANEL_PADDING, UI_PANEL_PADDING);
	section_size = init_v2f(size.x - (2 * UI_PANEL_PADDING), section_heights.x);
	attach_child(panel, create_camera_section(ctx, scene, section_pos, section_size));
	section_pos.y += section_heights.x + UI_PANEL_PADDING;
	section_size.y = section_heights.y;
	attach_child(panel, create_light_section(ctx, scene, section_pos, section_size));
	section_pos.y += section_heights.y + UI_PANEL_PADDING;
	section_size.y = section_heights.z;
	attach_child(panel, create_ambient_section(ctx, scene, section_pos, section_size));
	return (panel);
}
