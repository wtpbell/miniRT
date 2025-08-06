/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 17:32:35 by bewong           ###   ########.fr       */
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

static t_light *find_ambient_light(t_scene *scene)
{
	t_light *light;
	
	for (int i = 0; i < (int)scene->lights.size; i++)
	{
		light = (t_light *)vector_get(&scene->lights, i);
		if (light && light->type == LIGHT_AMBIENT)
			return light;
	}
	return NULL;
}

static void init_value_button_data(t_ui_value_button *value_btn, float *value,
	t_v2f range, float step, char *(*formatter)(float))
{
	value_btn->value = value;
	value_btn->range = range;
	value_btn->step = step;
	value_btn->on_click = NULL;
	value_btn->param = NULL;
	value_btn->formatter = formatter;
}

t_ui_element *create_value_button(t_ui_context *ctx, float *value,
	t_v2f range, float step, t_v2f pos, t_v2f size, char *(*formatter)(float))
{
	t_ui_element		*container;
	t_ui_element		*decr_btn;
	t_ui_element		*incr_btn;
	t_ui_element		*value_label;
	t_ui_value_button	*value_btn;
	char				*value_str;
	float				label_width;
	float				label_x;
	
	container = create_panel(ctx, pos, size);
	if (!container)
		return (NULL);
	value_btn = (t_ui_value_button *)ft_calloc(1, sizeof(t_ui_value_button));
	if (!value_btn)
		return (container);
	init_value_button_data(value_btn, value, range, step, formatter);
	decr_btn = create_button(ctx, "-", init_v2f(0, 0), init_v2f(UI_BUTTON_WIDTH, size.y),
		decrement_value_button, ctx);
	if (decr_btn)
		attach_child(container, decr_btn);
	incr_btn = create_button(ctx, "+", init_v2f(size.x - UI_BUTTON_WIDTH, 0), init_v2f(UI_BUTTON_WIDTH, size.y),
		increment_value_button, ctx);
	if (incr_btn)
		attach_child(container, incr_btn);
	if (formatter)
		value_str = ft_strdup(formatter(*value));
	else
		value_str = ft_ftoa(*value, 2);
	if (value_str)
	{
		label_width = ft_strlen(value_str) * UI_CHAR_WIDTH;
		label_x = (size.x - label_width) / 2.0f;
		value_label = create_label(ctx, value_str, init_v2f(label_x, (size.y - UI_FONT_HEIGHT) / 2), UI_TEXT_COLOR);
		if (value_label)
		{
			value_btn->value_label = value_label;
			attach_child(container, value_label);
		}
		free(value_str);
	}
	container->type = UI_VALUE_BUTTON;
	container->data = value_btn;
	return (container);
}

static char*	format_color_value(float value)
{
	static char	buf[16];
	
	snprintf(buf, sizeof(buf), "%6.2f", value * 255.0f);
	return (buf);
}

static t_ui_element	*create_labeled_control(t_ui_context *ctx, const char *label_text,
	float *value, t_v2f range, float step, t_v2f pos, float width, char *(*formatter)(float))
{
	float			label_width;
	float			control_width;
	float			control_x;
	t_ui_element	*container;
	t_ui_element	*label;
	t_ui_element	*control;
	
	container = create_panel(ctx, pos, init_v2f(width, UI_ROW_HEIGHT));
	if (!container)
		return (NULL);
	label_width = width * UI_LABEL_WIDTH_RATIO;
	control_width = width - label_width - UI_PADDING;
	control_x = label_width + UI_PADDING;
	label = create_label(ctx, label_text,
		init_v2f(UI_LABEL_PADDING, (UI_ROW_HEIGHT - UI_FONT_HEIGHT) / 2),
		UI_LABEL_COLOR);
	if (label)
		attach_child(container, label);
	control = create_value_button(ctx, value, range, step,
		init_v2f(control_x, 0),
		init_v2f(control_width, UI_ROW_HEIGHT),
		formatter);
	if (control)
		attach_child(container, control);
	return (container);
}

t_ui_element *create_ambient_section(t_ui_context *ctx, t_scene *scene,
	t_v2f pos, t_v2f size)
{
	const char *color_labels[] = {"COLOR R", "COLOR G", "COLOR B"};
	t_ui_element	*section;
	t_light			*ambient;
	t_ui_element	*header;
	float			current_y;
	float			content_width;
	float			*color_components[3];
	int				i;
	t_ui_element	*control;
	t_ui_element	*intensity;
	
	section = create_panel(ctx, pos, size);
	if (!section)
		return NULL;
	ambient = find_ambient_light(scene);
	if (!ambient)
		return (section);
	header = create_header(ctx, "AMBIENT LIGHT",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT));
	if (header)
		attach_child(section, header);
	current_y = UI_HEADER_HEIGHT + UI_PADDING;
	content_width = size.x - (UI_PADDING * 2);
	color_components[0] = &ambient->color.x;
	color_components[1] = &ambient->color.y;
	color_components[2] = &ambient->color.z;
	i = 0;
	while (i < 3)
	{
		control = create_labeled_control(
			ctx, color_labels[i], color_components[i],
			init_v2f(0, 1.0f), 1.0f/255.0f,
			init_v2f(UI_PADDING, current_y),
			content_width,
			format_color_value
		);
		if (control)
		{
			attach_child(section, control);
			current_y += UI_ROW_HEIGHT + UI_PADDING;
		}
		i++;
	}
	intensity = create_labeled_control(
		ctx, "INTENSITY", &ambient->intensity,
		init_v2f(0, 1), 0.05f,
		init_v2f(UI_PADDING, current_y),
		content_width,
		NULL
	);
	if (intensity)
		attach_child(section, intensity);
	return (section);
}
