/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/07/31 00:16:16 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

// Icon button creation removed as it's not currently used in the codebase

t_ui_element	*create_header(t_ui_context *ctx, const char *title,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_element	*title_label;

	header = create_panel(ctx, pos, size);
	if (!header)
		return (NULL);
	header->type = UI_HEADER;
	header->style.bg_color = UI_HEADER_COLOR;
	header->style.border_color = UI_BORDER_COLOR;

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

	container = create_panel(ctx, pos, size);
	if (!container)
		return (NULL);
	container->type = UI_VALUE_BUTTON;
	container->style.bg_color = UI_PANEL_BG_COLOR;
	container->style.border_color = UI_BUTTON_BORDER_COLOR;

	decr_btn = create_button(ctx, "-", init_v2f(0, 0), 
		init_v2f(30, size.y), NULL, NULL);
	if (decr_btn)
	{
		decr_btn->style.bg_color = UI_BUTTON_COLOR;
		decr_btn->style.border_color = 0x666666FF;
		attach_child(container, decr_btn);
	}
	value_str = ft_itoa((int)*value);
	if (value_str)
	{
		float label_x = (30 + (size.x - 60)) / 2.0f - (ft_strlen(value_str) * UI_CHAR_WIDTH) / 2.0f;
		value_label = create_label(ctx, value_str, 
			init_v2f(label_x, (size.y - UI_FONT_HEIGHT) / 2), 0xFFFFFFFF);
		if (value_label)
		{
			attach_child(container, value_label);
		}
		free(value_str);
	}
	(void)range;
	(void)step;

	incr_btn = create_button(ctx, "+", init_v2f(size.x - 60, 0), 
		init_v2f(30, size.y), NULL, NULL);
	if (incr_btn)
	{
		incr_btn->style.bg_color = UI_BUTTON_COLOR;
		incr_btn->style.border_color = 0x666666FF;
		attach_child(container, incr_btn);
	}

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

	section = create_panel(ctx, pos, size);
	if (!section)
		return (NULL);
	section->style.bg_color = UI_SECTION_COLOR;
	section->style.border_color = 0x00000000;

	ambient = NULL;
	for (i = 0; i < (int)scene->lights.size; i++)
	{
		ambient = (t_light *)vector_get(&scene->lights, i);
		if (ambient && ambient->type == LIGHT_AMBIENT)
			break;
	}
	if (!ambient)
	{
		fprintf(stderr, "No ambient light found\n");
		return (section);
	}
	header = create_header(ctx, "AMBIENT LIGHT",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT));
	if (header)
	{
		header->pos = init_v2f(0, 0);
		attach_child(section, header);
	}
	current_y = UI_HEADER_HEIGHT + 15;
	row_height = 30.0f;
	components[0] = &ambient->color.x;
	components[1] = &ambient->color.y;
	components[2] = &ambient->color.z;
	i = 0;
	while (i < 3)
	{
		label_elem = create_label(ctx, labels[i], 
			init_v2f(10, current_y + 5), 0xFFFFFFFF);
		if (label_elem)
		{
			label_elem->pos = init_v2f(10, current_y + 5);
			attach_child(section, label_elem);
		}
		intensity_control = create_value_button(ctx, components[i],
			init_v2f(0, 255), 1.0f,
			init_v2f(UI_BUTTON_WIDTH / 2, current_y),
			init_v2f(size.x - 100, row_height));
		if (intensity_control)
		{
			intensity_control->pos = init_v2f(UI_BUTTON_WIDTH / 2, current_y);
			attach_child(section, intensity_control);
		}
		current_y += row_height + 5;
		i++;
	}
	intensity_label = create_label(ctx, "INTENSITY",
		init_v2f(10, current_y + 5), 0xFFFFFFFF);
	if (intensity_label)
	{
		intensity_label->pos = init_v2f(10, current_y + 5);
		attach_child(section, intensity_label);
	}
	intensity_control = create_value_button(ctx, &ambient->intensity,
		init_v2f(0, 1), 0.05f, 
		init_v2f(UI_BUTTON_WIDTH / 2, current_y),
		init_v2f(size.x - 100, row_height));
	if (intensity_control)
	{
		intensity_control->pos = init_v2f(UI_BUTTON_WIDTH / 2, current_y);
		attach_child(section, intensity_control);
	}
	return (section);
}
