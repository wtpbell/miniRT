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

static void	(*const g_default_stylers[])(t_ui_element *, t_v2f, t_v2f) = {
	[UI_PANEL] = default_panel,
	[UI_BUTTON] = default_button,
	[UI_LABEL] = default_label,
	[UI_HEADER] = default_header,
	[UI_SECTION] = default_section,
};

t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = ft_calloc(1, sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->visible = true;
	element->style = (t_ui_style){
		.bg_color = UI_TRANSPARENT,
		.border_color = UI_TRANSPARENT,
		.text_color = UI_TEXT_COLOR,
		.padding = UI_PANEL_PADDING,
		.visible = true
	};
	if (type < (sizeof(g_default_stylers) / sizeof(g_default_stylers[0])) && 
		g_default_stylers[type] != NULL)
		g_default_stylers[type](element, pos, size);
	return (element);
}

t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;

	(void)ctx;
	panel = create_ui_element(UI_PANEL, pos, size);
	if (!panel)
		return (NULL);
	default_panel(panel, pos, size);
	panel->style.bg_color = UI_PANEL_BG_COLOR;
	return (panel);
}

t_ui_element	*create_button(t_ui_context *ctx, const char *label_text, t_v2f pos, t_v2f size,
	void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element	*button;
	t_ui_btn		*btn_data;

	(void)ctx;
	button = create_ui_element(UI_BUTTON, pos, size);
	if (!button)
		return (NULL);
	default_button(button, pos, size);
	button->action = on_click;
	button->data = ft_calloc(1, sizeof(t_ui_btn));
	if (!button->data)
		return (free(button), NULL);
	btn_data = (t_ui_btn *)button->data;
	btn_data->on_click = on_click;
	btn_data->param = param;
	btn_data->label = ft_strdup(label_text);
	if (!btn_data->label)
		return (free(button->data), free(button), NULL);
	button->style.text_color = UI_TEXT_COLOR;
	return (button);
}

t_ui_element	*create_rerender_button(t_ui *ui, t_v2f pos, t_v2f size)
{
	t_ui_element	*btn;

	btn = create_button(ui, "RE_RENDER", pos, size, render, ui->context);
	btn->data = ui;
	return (btn);
}


t_ui_element	*create_label(t_ui_context *ctx, const char *text, t_v2f pos, uint32_t color)
{
	t_ui_element	*label_elem;
	t_ui_label		*label_data;

	(void)ctx;
	label_elem = create_ui_element(UI_LABEL, pos, init_v2f(0, 0));
	if (!label_elem)
		return (NULL);
	default_label(label_elem, pos, init_v2f(0, 0));
	label_elem->style.text_color = color;
	label_elem->data = ft_calloc(1, sizeof(t_ui_label));
	if (!label_elem->data)
	{
		free(label_elem);
		return (NULL);
	}
	label_data = (t_ui_label *)label_elem->data;
	label_data->text = ft_strdup(text);
	if (!label_data->text)
		return (free(label_elem->data), free(label_elem), NULL);
	label_elem->size.x = ft_strlen(text) * UI_CHAR_WIDTH;
	label_elem->size.y = UI_CHAR_HEIGHT;
	return (label_elem);
}

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

static t_ui_element	*create_value_button(t_vbtn_config *cfg)
{
	t_ui_element	*container;
	t_ui_vbtn		*value_btn;

	container = create_panel(cfg->ctx, cfg->pos, cfg->size);
	if (!container)
		return (NULL);
	value_btn = (t_ui_vbtn *)ft_calloc(1, sizeof(t_ui_vbtn));
	if (!value_btn)
		return container;
	init_value_button_data(value_btn, cfg);
	add_inc_dec_buttons(container, cfg, value_btn);
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

