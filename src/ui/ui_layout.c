/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_layout.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 11:39:13 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/29 17:16:38 by bewong        ########   odam.nl         */
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

t_ui_element	*create_panel(mlx_t *mlx, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;
	mlx_image_t		*panel_img;
	uint32_t		x;
	uint32_t		y;
	uint32_t		color;

	panel = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!panel)
		return (NULL);
	default_panel(panel, pos, size);
	panel_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!panel_img)
		return (free(panel), NULL);
	y = 0;
	while (y < (uint32_t)size.y)
	{
		x = 0;
		while (x < (uint32_t)size.x)
		{
			if (x < 2 || y < 2 || x >= (uint32_t)size.x - 2 || y >= (uint32_t)size.y - 2)
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
		free(panel);
		return (NULL);
	}
	return (panel);
}

t_ui_element *create_label(mlx_t *mlx, const char *text, t_v2f pos, uint32_t color)
{
	t_v2f			size = init_v2f((float)(ft_strlen(text) * 8 + 10), 24.0f);
	t_ui_element	*label = ui_element_create(UI_LABEL, pos, size);
	mlx_image_t		*label_img;

	if (!label)
		return (NULL);
	label->style.text_color = color;
	label_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!label_img)
		return (free(label), NULL);
	draw_text(label_img, text, init_v2f(5, 5), color);
	if (!apply_instance(label, label_img, mlx, (int)pos.x, (int)pos.y))
		return (mlx_delete_image(mlx, label_img), free(label), NULL);
	label->layout_offset = pos;
	return label;
}


t_ui_element	*create_header(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_label		*label;
	mlx_image_t		*img;

	header = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!header)
		return (NULL);
	default_header(header, pos, size);
	label = (t_ui_label *)ft_calloc(1, sizeof(t_ui_label));
	if (!label)
		return (free(header), NULL);
	label->text = ft_strdup(title);
	label->color = UI_TEXT_COLOR;
	header->data = label;
	img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
		return (free(header), free(label), NULL);
	header->image = img;
	for (uint32_t y = 0; y < (uint32_t)size.y; ++y)
		for (uint32_t x = 0; x < (uint32_t)size.x; ++x)
			mlx_put_pixel(img, x, y, UI_HEADER_COLOR);
	draw_text(img, title, init_v2f(5, 5), UI_TEXT_COLOR);
	if (mlx_image_to_window(mlx, img, (int)pos.x, (int)pos.y) < 0)
		return (free(header), free(label), free(img), NULL);
	return (header);
}

static t_ui_element *create_icon_button(mlx_t *mlx, const char *text, t_v2f size, uint32_t color)
{
	t_ui_element *button = create_button(mlx, NULL, init_v2f(0, 0), size, NULL, NULL);
	if (!button)
		return (NULL);
	t_ui_element *label = create_label(mlx, text,
		init_v2f((size.x - ft_strlen(text) * 8) / 2, (size.y - UI_FONT_HEIGHT) / 2),
		color);
	if (label)
		attach_child(button, label);
	return button;
}


t_ui_element *create_value_button(mlx_t *mlx, const char *label, float *value,
	t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element		*container;
	t_ui_value_button	*val_btn;
	t_ui_element		*label_elem;
	t_ui_element		*row, *minus_btn, *plus_btn, *value_display;
	char				value_str[32];
	t_v2f				button_size = init_v2f(24, size.y);

	// Outer container to hold label and row
	container = ui_element_create(UI_VALUE_BUTTON, pos, init_v2f(size.x, size.y * 2));
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

	// Optional label above
	if (label && *label)
	{
		label_elem = create_label(mlx, label, init_v2f(0, 0), 0xFFFFFFFF);
		if (label_elem)
			attach_child(container, label_elem);
	}

	// Row: [-] [value] [+]
	row = ui_element_create(UI_PANEL, init_v2f(0, size.y), init_v2f(size.x, size.y));
	if (!row)
		return (ui_element_destroy(container, mlx, true), NULL);

	// - Button
	minus_btn = create_icon_button(mlx, "-", button_size, 0xFFAAAAAA);
	if (minus_btn)
	{
		minus_btn->data = val_btn;
		minus_btn->action = decrement_value_button;
		attach_child(row, minus_btn);
	}

	// Value label
	snprintf(value_str, sizeof(value_str), "%.2f", *value);
	value_display = create_label(mlx, value_str, init_v2f(0, 0), 0xFFFFFFAA);
	if (value_display)
	{
		val_btn->value_label = value_display;
		attach_child(row, value_display);
	}

	// + Button
	plus_btn = create_icon_button(mlx, "+", button_size, 0xFFAAAAAA);
	if (plus_btn)
	{
		plus_btn->data = val_btn;
		plus_btn->action = increment_value_button;
		attach_child(row, plus_btn);
	}

	attach_child(container, row);
	layout_horizontal(row, 5); // Optional spacing between -, value, +

	return container;
}



t_ui_element	*create_button(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
						void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element	*button = ui_element_create(UI_BUTTON, pos, size);
	t_ui_button		*btn_data;
	mlx_image_t		*img;

	if (!button)
		return (NULL);

	img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
		return (free(button), NULL);

	// Draw border and background
	for (uint32_t y = 0; y < (uint32_t)size.y; ++y)
	{
		for (uint32_t x = 0; x < (uint32_t)size.x; ++x)
		{
			uint32_t color = (x < 2 || y < 2 || x >= (uint32_t)size.x - 2 || y >= (uint32_t)size.y - 2)
							 ? 0xFFFFFFFF : 0xFF555555;
			mlx_put_pixel(img, x, y, color);
		}
	}

	if (!apply_instance(button, img, mlx, (int)pos.x, (int)pos.y))
	{
		mlx_delete_image(mlx, img);
		return (free(button), NULL);
	}

	btn_data = (t_ui_button *)ft_calloc(1, sizeof(t_ui_button));
	if (!btn_data)
		return (ui_element_destroy(button, mlx, true), NULL);
	btn_data->on_click = on_click;
	btn_data->param = param;
	button->data = btn_data;

	// Add label centered in the button
	if (label && *label)
	{
		t_ui_element *label_elem = create_label(mlx, label, init_v2f(0, 0), 0xFFFFFFFF);
		if (label_elem)
		{
			label_elem->layout_offset = init_v2f(
				(size.x - ft_strlen(label) * 8) / 2.0f,
				(size.y - UI_FONT_HEIGHT) / 2.0f
			);
			attach_child(button, label_elem);
		}
	}
	return (button);
}

t_ui_element *create_ambient_section(mlx_t *mlx, t_scene *scene, t_v2f pos, t_v2f size)
{
	const float		spacing = 5;
	const t_v2f		button_size = init_v2f(size.x - 20, 25);
	const char		*labels[] = {"COL R", "COL G", "COL B"};
	t_ui_element	*section = create_panel(mlx, pos, size);
	t_light			*ambient = NULL;
	t_ui_element	*header, *child;
	int				i = 0;

	if (!section)
		return (NULL);

	// Find ambient light
	while (i < (int)scene->lights.size)
	{
		t_light *l = (t_light *)vector_get(&scene->lights, i);
		if (l && l->type == LIGHT_AMBIENT)
		{
			ambient = l;
			break;
		}
		++i;
	}
	if (!ambient)
		return (fprintf(stderr, "No ambient light found\n"), section);

	// Header
	header = create_header(mlx, "AMBIENT LIGHT", init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT));
	if (header)
	{
		header->layout_offset = init_v2f(0, 5);
		attach_child(section, header);
	}

	child = create_label(mlx, "INTENSITY", init_v2f(0, 0), UI_TEXT_COLOR);

	if (child)
	{
		child->layout_offset = init_v2f(10, 0);
		attach_child(section, child);
	}

	// Intensity Value Button
	child = create_value_button(mlx, NULL, &ambient->intensity, init_v2f(0, 1), 0.05f,
								init_v2f(0, 0), button_size);
	if (child)
	{
		child->layout_offset = init_v2f(10, 0);
		attach_child(section, child);
	}

	// RGB Controls (Label + Value Button for each)
	float *components[] = {&ambient->color.x, &ambient->color.y, &ambient->color.z};
	i = 0;
	while (i < 3)
	{
		// RGB Label
		child = create_label(mlx, labels[i],  init_v2f(0, 0), 0xFFFFFFFF);
		if (child)
		{
			child->layout_offset = init_v2f(10, 0);
			attach_child(section, child);
		}

		// RGB Value Button
		child = create_value_button(mlx, NULL, components[i], init_v2f(0, 255), 1.0f,
									init_v2f(0, 0), button_size);
		if (child)
		{
			child->layout_offset = init_v2f(100, 0);
			attach_child(section, child);
		}
		++i;
	}
	
	layout_vertical(section, spacing);
	return section;
}



