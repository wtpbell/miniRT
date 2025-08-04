/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/08/04 21:10:26 by bewong           ###   ########.fr       */
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

t_ui_element	*create_value_button(t_ui_context *ctx, float *value,
	t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element	*container;
	t_ui_element	*decr_btn;
	t_ui_element	*incr_btn;
	t_ui_element	*value_label;
	char		*value_str;
	float		btn_width = 30.0f;

	container = create_panel(ctx, pos, size);
	if (!container)
		return (NULL);

	// Create - button on the left
	decr_btn = create_button(ctx, "-", 
		init_v2f(0, 0), 
		init_v2f(btn_width, size.y), 
		NULL, NULL);

	// Create + button on the right
	incr_btn = create_button(ctx, "+", 
		init_v2f(size.x - btn_width, 0), 
		init_v2f(btn_width, size.y), 
		NULL, NULL);

	// Create value label in the center
	value_str = ft_itoa((int)*value);
	if (value_str)
	{
		float label_width = ft_strlen(value_str) * UI_CHAR_WIDTH;
		float label_x = (size.x - label_width) / 2.0f;
		
		value_label = create_label(ctx, value_str, 
			init_v2f(label_x, (size.y - UI_FONT_HEIGHT) / 2), 
			0xFFFFFFFF);
		
		if (value_label)
		{
			attach_child(container, value_label);
		}
		free(value_str);
	}

	// Attach buttons
	if (decr_btn)
	{
		attach_child(container, decr_btn);
	}
	if (incr_btn)
	{
		attach_child(container, incr_btn);
	}
	(void)range;
	(void)step;
	return (container);
}

t_ui_element	*create_ambient_section(t_ui_context *ctx, t_scene *scene,
				t_v2f pos, t_v2f size)
{
	const char		*labels[] = {"COL R", "COL G", "COL B"};
	t_ui_element	*section;
	t_light			*ambient;
	t_ui_element	*header;
	int				i;
	float			current_y;
	float			row_height;
	float			*components[3];
	t_ui_element	*intensity_label;
	t_ui_element	*intensity_control;
	t_ui_element	*label_elem;
	// Calculate dimensions using macros from ui.h
	const float		label_width = size.x * UI_LABEL_WIDTH_RATIO;
	const float		button_width = size.x * UI_BTN_WIDTH_RATIO;
	const float		button_height = size.y * UI_BTN_HEIGHT_RATIO;
	const float		v_padding = size.y * UI_VERT_PADDING_RATIO;
	const float		h_padding = size.x * UI_HORZ_PADDING_RATIO;

	section = create_panel(ctx, pos, size);
	if (!section)
		return (NULL);
	ambient = NULL;
	for (i = 0; i < (int)scene->lights.size; i++)
	{
		ambient = (t_light *)vector_get(&scene->lights, i);
		if (ambient && ambient->type == LIGHT_AMBIENT)
			break;
	}
	if (!ambient)
		return (section);
	header = create_header(ctx, "AMBIENT LIGHT",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT));
	if (header)
		attach_child(section, header);
	// Calculate starting position below header
	current_y = UI_HEADER_HEIGHT + v_padding * 2;
	row_height = button_height;
	components[0] = &ambient->color.x;
	components[1] = &ambient->color.y;
	components[2] = &ambient->color.z;
	i = 0;
	while (i < 3)
	{
		// Position the label
		label_elem = create_label(ctx, labels[i], 
			init_v2f(h_padding, 
				current_y + (row_height - UI_FONT_HEIGHT) / 2), 
			0xFFFFFFFF);
		if (label_elem)
			attach_child(section, label_elem);
		
		// Position the value button
		intensity_control = create_value_button(ctx, components[i],
			init_v2f(0, 255), 1.0f,
			init_v2f(h_padding + label_width, current_y),
			init_v2f(button_width, row_height));
		if (intensity_control)
			attach_child(section, intensity_control);
		
		current_y += row_height + 5;
		i++;
	}
	
	// Position the INTENSITY label
	intensity_label = create_label(ctx, "INTENSITY",
		init_v2f(h_padding, 
			current_y + (row_height - UI_FONT_HEIGHT) / 2), 
		0xFFFFFFFF);
	if (intensity_label)
		attach_child(section, intensity_label);
		
	// Position the intensity button
	intensity_control = create_value_button(ctx, &ambient->intensity,
		init_v2f(0, 1), 0.05f, 
		init_v2f(h_padding + label_width, current_y),
		init_v2f(button_width, row_height));
	if (intensity_control)
		attach_child(section, intensity_control);
	return (section);
}
