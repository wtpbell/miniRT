/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:05:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 16:05:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

void	init_value_button_data(t_ui_vbtn *value_btn, const t_vbtn_config *cfg)
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

void	add_inc_dec_buttons(t_ui_element *container, const t_vbtn_config *cfg)
{
	t_btn_config	dc;
	t_btn_config	ic;
	t_ui_element	*btn;

	dc.ctx = cfg->ctx;
	dc.label_text = "-";
	dc.pos = g_v2f_zero;
	dc.size = init_v2f(UI_BUTTON_WIDTH, cfg->size.y);
	dc.on_click = decrement_value_button;
	dc.param = (void *)cfg->ctx;
	ic.ctx = cfg->ctx;
	ic.label_text = "+";
	ic.pos = init_v2f(cfg->size.x - UI_BUTTON_WIDTH, 0);
	ic.size = init_v2f(UI_BUTTON_WIDTH, cfg->size.y);
	ic.on_click = increment_value_button;
	ic.param = (void *)cfg->ctx;
	btn = create_button(&dc);
	if (btn)
		attach_child(container, btn);
	btn = create_button(&ic);
	if (btn)
		attach_child(container, btn);
}

static t_ui_element	*create_value_label_element(t_ui_context *ctx,
	const char *text, t_v2f size)
{
	float	label_width;
	float	label_x;
	t_v2f	pos;

	label_width = ft_strlen(text) * UI_CHAR_WIDTH;
	label_x = (size.x - label_width) / 2.0f;
	pos = init_v2f(label_x, (size.y - UI_FONT_HEIGHT) / 2);
	return (create_label(ctx, text, pos, UI_TEXT_COLOR));
}

void	add_value_label(t_ui_element *container,
	const t_vbtn_config *cfg, t_ui_vbtn *value_btn)
{
	const char		*value_str;
	char			buf[32];
	t_ui_element	*label;

	if (cfg->formatter)
		value_str = cfg->formatter(*cfg->value);
	else
	{
		rt_snprintf(buf, sizeof(buf), "%f", *cfg->value);
		value_str = buf;
	}
	label = create_value_label_element(cfg->ctx, value_str, cfg->size);
	if (label)
	{
		if (value_btn->value_label)
		{
			destroy_ui_element(value_btn->value_label);
			value_btn->value_label = NULL;
		}
		value_btn->value_label = label;
		attach_child(container, label);
	}
}
