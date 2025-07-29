/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/07/29 23:14:55 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

bool	apply_instance(t_ui_element *element, mlx_image_t *img,
			mlx_t *mlx, int32_t x, int32_t y)
{
	mlx_instance_t	*inst;

	element->image = img;
	element->pos = init_v2f(x, y);
	if (element->instance_id >= 0 && element->instance_id < (int32_t)img->count)
	{
		inst = &img->instances[element->instance_id];
		inst->x = x;
		inst->y = y;
		return (true);
	}
	element->instance_id = mlx_image_to_window(mlx, img, x, y);
	return (element->instance_id >= 0);
}

static t_ui_element	*create_icon_button(mlx_t *mlx, const char *text,
	t_v2f size)
{
	t_v2f			btn_size;
	t_ui_element	*button;
	t_ui_element	*label;

	btn_size = init_v2f(fmaxf(40.0f, size.x), fmaxf(30.0f, size.y));
	button = create_button(mlx, NULL, init_v2f(0, 0), btn_size, NULL, NULL);
	if (!button)
		return (NULL);
	button->style.bg_color = 0x333333FF;
	button->style.border_color = 0x666666FF;
	label = create_label(mlx, text, init_v2f(0, 0), 0xFFFFFFFF);
	if (label)
	{
		label->pos = init_v2f(
		(btn_size.x - ft_strlen(text) * 8) / 2,
		(btn_size.y - UI_FONT_HEIGHT) / 2);
		label->style.text_color = 0xFFFFFFFF;
		attach_child(button, label);
	}
	return (button);
}

t_ui_element	*create_panel(mlx_t *mlx, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;
	mlx_image_t		*panel_img;
	uint32_t		x;
	uint32_t		y;
	uint32_t		color;

	panel = ui_element_create(UI_PANEL, pos, size);
	if (!panel)
		return (NULL);
	panel_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!panel_img)
		return (ui_element_destroy(panel, mlx, true), NULL);
	y = 0;
	while (y < (uint32_t)size.y)
	{
		x = 0;
		while (x < (uint32_t)size.x)
		{
			if (x < 2 || y < 2 || x >= (uint32_t)size.x - 2
				|| y >= (uint32_t)size.y - 2)
				color = UI_BORDER_COLOR;
			else
				color = panel->style.bg_color;
			mlx_put_pixel(panel_img, x, y, color);
			x++;
		}
		y++;
	}
	if (!apply_instance(panel, panel_img, mlx, (int)pos.x, (int)pos.y))
	{
		mlx_delete_image(mlx, panel_img);
		return (ui_element_destroy(panel, mlx, true), NULL);
	}
	return (panel);
}

t_ui_element	*create_label(mlx_t *mlx, const char *text, t_v2f pos,
					uint32_t color)
{
	t_v2f			size;
	t_ui_element	*label;
	mlx_image_t		*label_img;

	size = init_v2f((float)(ft_strlen(text) * 8 + 10), 24.0f);
	label = ui_element_create(UI_LABEL, pos, size);
	if (!label)
		return (NULL);
	label->style.text_color = color;
	label_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!label_img)
		return (ui_element_destroy(label, mlx, true), NULL);
	draw_text(label_img, text, init_v2f(5, 5), color);
	if (!apply_instance(label, label_img, mlx, (int)pos.x, (int)pos.y))
	{
		mlx_delete_image(mlx, label_img);
		return (ui_element_destroy(label, mlx, true), NULL);
	}
	label->layout_offset = pos;
	return (label);
}

t_ui_element	*create_header(mlx_t *mlx, const char *title,
					t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_label		*label;
	mlx_image_t		*img;
	uint32_t		x;
	uint32_t		y;
	float			text_width;
	float			text_height;
	float			text_x;
	float			text_y;

	header = ui_element_create(UI_HEADER, pos, size);
	if (!header)
		return (NULL);
	label = ft_calloc(1, sizeof(t_ui_label));
	if (!label)
		return (ui_element_destroy(header, mlx, true), NULL);
	label->text = ft_strdup(title);
	label->color = UI_TEXT_COLOR;
	header->data = label;
	img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
	{
		free(label->text);
		free(label);
		return (ui_element_destroy(header, mlx, true), NULL);
	}
	y = 0;
	while (y < (uint32_t)size.y)
	{
		x = 0;
		while (x < (uint32_t)size.x)
		{
			mlx_put_pixel(img, x, y, UI_HEADER_COLOR);
			x++;
		}
		y++;
	}
	text_width = ft_strlen(title) * 8;
	text_height = 12;
	text_x = (size.x - text_width) / 2;
	text_y = (size.y - text_height) / 2;
	text_x = fmax(5, fmin(text_x, size.x - text_width - 5));
	text_y = fmax(2, fmin(text_y, size.y - text_height - 2));
	draw_text(img, title, init_v2f(text_x, text_y), UI_TEXT_COLOR);
	if (mlx_image_to_window(mlx, img, (int)pos.x, (int)pos.y) < 0)
	{
		mlx_delete_image(mlx, img);
		free(label->text);
		free(label);
		return (ui_element_destroy(header, mlx, true), NULL);
	}
	header->image = img;
	return (header);
}

t_ui_element	*create_value_button(mlx_t *mlx, float *value,
					t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element		*container;
	t_ui_value_button	*val_btn;
	t_ui_element		*row;
	t_ui_element		*minus_btn;
	t_ui_element		*plus_btn;
	t_ui_element		*value_display;
	char				value_str[32];
	t_v2f				button_size;

	button_size = init_v2f(30, size.y);
	container = ui_element_create(UI_VALUE_BUTTON, pos, size);
	if (!container)
		return (NULL);
	val_btn = ft_calloc(1, sizeof(t_ui_value_button));
	if (!val_btn)
		return (ui_element_destroy(container, mlx, true), NULL);
	*value = fmaxf(range.x, fminf(range.y, *value));
	val_btn->value = value;
	val_btn->range = range;
	val_btn->step = step;
	container->data = val_btn;
	row = ui_element_create(UI_PANEL, pos, size);
	if (!row)
		return (ui_element_destroy(container, mlx, true), NULL);
	container->pos = pos;
	minus_btn = create_icon_button(mlx, "-", button_size);
	if (minus_btn)
	{
		minus_btn->data = val_btn;
		minus_btn->action = decrement_value_button;
		attach_child(row, minus_btn);
	}
	snprintf(value_str, sizeof(value_str), "%.2f", *value);
	value_display = create_label(mlx, value_str,
			init_v2f(0, (button_size.y - 16.0f) / 2.0f), 0xFFFFFFAA);
	if (value_display)
	{
		val_btn->value_label = value_display;
		attach_child(row, value_display);
	}
	plus_btn = create_icon_button(mlx, "+", button_size);
	if (plus_btn)
	{
		plus_btn->data = val_btn;
		plus_btn->action = increment_value_button;
		attach_child(row, plus_btn);
	}
	row->layout_offset = init_v2f(10, 0);
	attach_child(container, row);
	layout_horizontal(row, 5);
	if (container->image)
	{
		container->image->instances[container->instance_id].x = (int)pos.x;
		container->image->instances[container->instance_id].y = (int)pos.y;
	}
	return (container);
}

t_ui_element	*create_button(mlx_t *mlx, const char *label, t_v2f pos,
					t_v2f size, void (*on_click)(t_ui_element *, void *),
					void *param)
{
	t_ui_element	*button;
	t_ui_button		*btn_data;
	mlx_image_t		*img;
	uint32_t		x;
	uint32_t		y;
	t_ui_element	*label_elem;

	button = ui_element_create(UI_BUTTON, pos, size);
	if (!button)
		return (NULL);
	img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
		return (ui_element_destroy(button, mlx, true), NULL);
	y = 0;
	while (y < (uint32_t)size.y)
	{
		x = 0;
		while (x < (uint32_t)size.x)
		{
			if (x < 2 || y < 2 || x >= (uint32_t)size.x - 2
				|| y >= (uint32_t)size.y - 2)
				mlx_put_pixel(img, x, y, 0xFFFFFFFF);
			else
				mlx_put_pixel(img, x, y, 0xFF555555);
			x++;
		}
		y++;
	}
	if (!apply_instance(button, img, mlx, (int)pos.x, (int)pos.y))
	{
		mlx_delete_image(mlx, img);
		return (ui_element_destroy(button, mlx, true), NULL);
	}
	button->needs_redraw = true;
	btn_data = ft_calloc(1, sizeof(t_ui_button));
	if (!btn_data)
		return (ui_element_destroy(button, mlx, true), NULL);
	btn_data->on_click = on_click;
	btn_data->param = param;
	button->data = btn_data;
	if (label && *label)
	{
		label_elem = create_label(mlx, label, init_v2f(0, 0), 0xFFFFFFFF);
		if (label_elem)
		{
			label_elem->layout_offset = init_v2f(
					(size.x - ft_strlen(label) * 8) / 2.0f,
					(size.y - UI_FONT_HEIGHT) / 2.0f);
			attach_child(button, label_elem);
		}
	}
	return (button);
}

t_ui_element	*create_ambient_section(mlx_t *mlx, t_scene *scene,
					t_v2f pos, t_v2f size)
{
	const char		*labels[] = {"COL R", "COL G", "COL B"};
	float			section_x;
	t_ui_element	*section;
	t_light			*ambient;
	t_ui_element	*header;
	int				i;
	float			current_y;
	float			row_height;
	float			*components[3];
	t_ui_element	*intensity_label;
	t_ui_element	*intensity_control;

	section_x = pos.x + (UI_PANEL_WIDTH - size.x) / 2.0f;
	section = ui_element_create(UI_PANEL, init_v2f(section_x, pos.y), size);
	section->style.bg_color = 0x00000000;
	section->style.border_color = 0x00000000;
	ambient = NULL;
	i = 0;
	while (i < (int)scene->lights.size)
	{
		ambient = (t_light *)vector_get(&scene->lights, i);
		if (ambient && ambient->type == LIGHT_AMBIENT)
			break;
		i++;
	}
	if (!ambient)
		return (fprintf(stderr, "No ambient light found\n"), section);
	header = create_header(mlx, "AMBIENT LIGHT",
			init_v2f(0, 0), init_v2f(UI_PANEL_WIDTH, UI_HEADER_HEIGHT));
	if (header)
	{
		header->layout_offset = init_v2f(0, 0);
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
		create_label(mlx, labels[i], init_v2f(10, current_y + 5), 0xFFFFFFFF);
		attach_child(section, create_value_button(mlx, components[i],
				init_v2f(0, 255), 1.0f,
				init_v2f(UI_BUTTON_WIDTH / 2, current_y),
				init_v2f(size.x - 100, row_height)));
		current_y += row_height + 5;
		i++;
	}
	intensity_label = create_label(mlx, "INTENSITY",
			init_v2f(10, current_y + 5), UI_TEXT_COLOR);
	if (intensity_label)
		attach_child(section, intensity_label);
	intensity_control = create_value_button(mlx, &ambient->intensity,
			init_v2f(0, 1), 0.05f, init_v2f(UI_BUTTON_WIDTH / 2, current_y),
			init_v2f(size.x - 100, row_height));
	if (intensity_control)
		attach_child(section, intensity_control);
	return (section);
}
