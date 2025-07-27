/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/07/27 23:50:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

bool	apply_instance(t_ui_element *element, mlx_image_t *img,
					mlx_t *mlx, int32_t x, int32_t y)
{
	element->image = img;
	element->pos = init_v2f(x, y);
	if (element->instance_id >= 0 && element->instance_id < (int32_t)img->count)
	{
		mlx_instance_t *inst = &img->instances[element->instance_id];
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
	panel->type = UI_PANEL;
	panel->pos = pos;
	panel->size = size;
	panel->visible = true;
	panel->style.bg_color = UI_PANEL_BG_COLOR;
	panel->style.border_color = UI_BORDER_COLOR;
	panel->style.text_color = UI_TEXT_COLOR;
	panel->style.padding = UI_PANEL_PADDING;
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
				color = 0x666666FF;
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

t_ui_element	*create_label(mlx_t *mlx, const char *text, t_v2f pos, uint32_t color)
{
	t_v2f			size;
	t_ui_element	*label;
	mlx_image_t		*label_img;

	size.x = (float)(ft_strlen(text) * 8 + 10);
	size.y = 24.0f;

	label = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!label)
		return (NULL);
	label->type = UI_LABEL;
	label->pos = pos;
	label->size = size;
	label->style.text_color = color;
	label_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!label_img)
		return (free(label), NULL);
	draw_text(label_img, text, init_v2f(5, 5), color);
	if (!apply_instance(label, label_img, mlx, (int)pos.x, (int)pos.y))
		return (mlx_delete_image(mlx, label_img), free(label), NULL);
	return (label);
}


t_ui_element	*create_header(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_label		*label;
	mlx_image_t		*img;

	header = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!header)
		return (NULL);
	header->type = UI_HEADER;
	header->pos = pos;
	header->size = size;
	header->visible = true;
	header->style.bg_color = UI_HEADER_COLOR;
	header->style.text_color = UI_TEXT_COLOR;
	header->style.padding = 10;
	header->style.border_color = UI_BORDER_COLOR;
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
	if (mlx_image_to_window(mlx, img, (int)pos.x, (int)pos.y) < 0)
		return (free(header), free(label), free(img), NULL);
	return (header);
}

t_ui_element	*create_value_button(mlx_t *mlx, const char *label, float *value,
							t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element		*button;
	t_ui_value_button	*val_btn;
	char				value_str[32];
	t_ui_element		*label_elem;
	t_ui_element		*value_elem;
	t_v2f				label_pos;

	button = create_button(mlx, "", pos, size, NULL, NULL);
	if (!button)
		return NULL;
	val_btn = (t_ui_value_button *)ft_calloc(1, sizeof(t_ui_value_button));
	if (!val_btn)
		return (ui_element_destroy(button, mlx, true), NULL);
	val_btn->value = value;
	val_btn->range = range;
	val_btn->step = step;
	*val_btn->value = fmaxf(range.x, fminf(range.y, *val_btn->value));
	button->data = val_btn;
	label_pos = init_v2f(5, (size.y - 12) / 2.0f);
	label_elem = create_label(mlx, label, label_pos, 0xFFFFFFFF);
	if (label_elem)
		attach_child(button, label_elem);
	snprintf(value_str, sizeof(value_str), "%.2f", *val_btn->value);
	snprintf(value_str, sizeof(value_str), "%.2f", *value);
	t_v2f value_pos = init_v2f(size.x - 60, (size.y - 12) / 2.0f);
	value_elem = create_label(mlx, value_str, value_pos, 0xFFFFFFAA);
	if (value_elem)
	{
		attach_child(button, value_elem);
		val_btn->value_label = value_elem;
	}
	return (button);
}

t_ui_element	*create_button(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
						void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element	*button;
	t_ui_element	*label_elem;
	t_ui_button		*btn_data;
	mlx_image_t		*button_img;
	uint32_t		color;
	uint32_t		x;
	uint32_t		y;
	t_v2f			label_pos;

	button = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!button)
		return (NULL);
	button->type = UI_BUTTON;
	button->pos = pos;
	button->size = size;
	button_img = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!button_img)
		return (free(button), NULL);
	y = 0;
	while (y < (uint32_t)size.y)
	{
		x = 0;
		while (x < (uint32_t)size.x)
		{
			color = 0xFF555555;
			if (x < 2 || y < 2 || x >= (uint32_t)size.x - 2 || y >= (uint32_t)size.y - 2)
				color = 0xFFFFFFFF;
			mlx_put_pixel(button_img, x, y, color);
			x++;
		}
		y++;
	}
	if (!apply_instance(button, button_img, mlx, (int)pos.x, (int)pos.y))
	{
		mlx_delete_image(mlx, button_img);
		free(button);
		return NULL;
	}
	btn_data = (t_ui_button *)ft_calloc(1, sizeof(t_ui_button));
	if (!btn_data)
	{
		mlx_delete_image(mlx, button_img);
		free(button);
		return NULL;
	}
	btn_data->on_click = on_click;
	btn_data->param = param;
	button->data = btn_data;
	if (label)
	{
		label_pos = init_v2f(
			(size.x - (ft_strlen(label) * 8)) / 2.0f,
			(size.y - 12) / 2.0f
		);
		label_elem = create_label(mlx, label, label_pos, 0xFFFFFFFF);
		if (label_elem)
			attach_child(button, label_elem);
	}

	return (button);
}

t_ui_element	*create_ambient_section(mlx_t *mlx, t_scene *scene, t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	float			button_width;
	float			button_height;
	float			y_offset;
	float			spacing;
	t_light			*ambient_light;
	int				i;
	t_light			*light;
	t_ui_element	*intensity_btn;

	section = ui_element_create(UI_SECTION, pos, size);
	if (!section)
	{
		fprintf(stderr, "Error: Failed to create ambient section\n");
		return NULL;
	}
	default_section(section, pos, size);

	button_width = size.x - 20;
	button_height = 25;
	y_offset = 40;
	spacing = 5;
	ambient_light = NULL;
	i = 0;
	while (i < (int)scene->lights.size)
	{
		light = (t_light *)vector_get(&scene->lights, i);
		if (light && light->type == LIGHT_AMBIENT)
		{
			ambient_light = light;
			break ;
		}
		i++;
	}
	if (!ambient_light)
	{
		fprintf(stderr, "Warning: No ambient light found in scene\n");
		return section;
	}
	intensity_btn = create_value_button(
		mlx, 
		"Intensity", 
		&ambient_light->intensity, 
		init_v2f(0.0f, 1.0f), 
		0.05f, 
		init_v2f(10, y_offset), 
		init_v2f(button_width, button_height)
	);
	if (intensity_btn)
	{
		attach_child(section, intensity_btn);
		y_offset += button_height + spacing;
	}

	// Add color controls for R, G, B components
	t_ui_element *r_btn = create_value_button(
		mlx,
		"R",
		&ambient_light->color.x,
		init_v2f(0.0f, 1.0f),
		0.05f,
		init_v2f(10, y_offset),
		init_v2f((button_width - 10) / 3, button_height)
	);
	
	t_ui_element *g_btn = create_value_button(
		mlx,
		"G",
		&ambient_light->color.y,
		init_v2f(0.0f, 1.0f),
		0.05f,
		init_v2f(15 + (button_width - 10) / 3, y_offset),
		init_v2f((button_width - 10) / 3, button_height)
	);
	
	t_ui_element *b_btn = create_value_button(
		mlx,
		"B",
		&ambient_light->color.z,
		init_v2f(0.0f, 1.0f),
		0.05f,
		init_v2f(20 + 2 * (button_width - 10) / 3, y_offset),
		init_v2f((button_width - 10) / 3, button_height)
	);

	if (r_btn) attach_child(section, r_btn);
	if (g_btn) attach_child(section, g_btn);
	if (b_btn) attach_child(section, b_btn);
	
	y_offset += button_height + spacing;
	t_ui_element *preview = create_panel(mlx, init_v2f(10, y_offset), init_v2f(button_width, button_height));
	if (preview)
	{
		preview->style.bg_color = (uint32_t)(ambient_light->color.x * 255) << 16 |
								 (uint32_t)(ambient_light->color.y * 255) << 8 |
								 (uint32_t)(ambient_light->color.z * 255) |
								 0xFF000000;
		attach_child(section, preview);
		y_offset += button_height + spacing;
	}
	t_ui_element *header = create_header(mlx, "Ambient Light", init_v2f(0, 5), init_v2f(size.x, UI_HEADER_HEIGHT));
	if (header)
		attach_child(section, header);

	return section;
}
