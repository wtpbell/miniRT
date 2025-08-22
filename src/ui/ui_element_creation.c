/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_element_creation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 20:30:21 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 20:30:21 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static t_ui_element	*create_value_button(t_vbtn_config *cfg)
{
	t_ui_element	*container;
	t_ui_vbtn		*value_btn;

	container = create_panel(cfg->ctx, cfg->pos, cfg->size);
	if (!container)
		return (NULL);
	value_btn = (t_ui_vbtn *)ft_calloc(1, sizeof(t_ui_vbtn));
	if (!value_btn)
		return (free(container), NULL);
	init_value_button_data(value_btn, cfg);
	add_inc_dec_buttons(container, cfg);
	add_value_label(container, cfg, value_btn);
	container->type = UI_VALUE_BUTTON;
	container->data = value_btn;
	return (container);
}

t_ui_element	*create_labeled_control(t_vbtn_config *cfg,
	const char *label_text, float total_width)
{
	float			label_width;
	float			control_width;
	t_ui_element	*container;
	t_ui_element	*label;
	t_ui_element	*value_btn;

	container = create_panel(cfg->ctx, cfg->pos,
			init_v2f(total_width, UI_ROW_HEIGHT));
	if (!container)
		return (NULL);
	label_width = total_width * UI_LABEL_WIDTH_RATIO;
	control_width = total_width - label_width - UI_PADDING;
	label = create_label(cfg->ctx, label_text, init_v2f(UI_LABEL_PADDING,
				(UI_ROW_HEIGHT - UI_FONT_HEIGHT) / 2), UI_LABEL_COLOR);
	if (!label)
		return (destroy_ui_element(container), NULL);
	attach_child(container, label);
	cfg->pos = init_v2f(label_width + UI_PADDING, 0);
	cfg->size = init_v2f(control_width, UI_ROW_HEIGHT);
	value_btn = create_value_button(cfg);
	if (!value_btn)
		return (destroy_ui_element(label), destroy_ui_element(container), NULL);
	attach_child(container, value_btn);
	container->style.bg_color = UI_TRANSPARENT;
	return (container);
}
