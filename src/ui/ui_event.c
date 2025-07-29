/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:51:30 by bewong            #+#    #+#             */
/*   Updated: 2025/07/29 22:57:47 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include "ui.h"

bool is_point_in_element(t_ui_element *element, int x, int y)
{
	
	return (x >= element->pos.x &&
		x <= (element->pos.x + element->size.x) &&
		y >= element->pos.y &&
		y <= (element->pos.y + element->size.y));
}

void handle_ui_click(t_ui_element *root, int32_t x, int32_t y)
{
	t_ui_element *child;

	if (!root || !is_point_in_element(root, x, y))
		return;
	if (root->type == UI_BUTTON && root->data)
		update_button_value(root, x);
	child = root->first_child;
	while (child)
	{
		handle_ui_click(child, x, y);
		child = child->next_sibling;
	}
}

void update_label_text(t_ui_element *label, const char *text)
{
	t_ui		*ui;
	mlx_image_t	*img;
	t_v2f		size;

	ui = (t_ui *)label->parent;
	if (!ui || !ui->mlx)
		return ;
	size = init_v2f(ft_strlen(text) * 8 + 10, 24);
	img = mlx_new_image(ui->mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
		return;
	draw_text(img, text, init_v2f(5, 5), label->style.text_color);
	label->image = img;
}

void update_button_value(t_ui_element *button, int32_t click_x)
{
	t_ui				*ui;
	t_ui_value_button	*value_btn;
	float				relative_x;
	float				button_mid;
	float				old_value;
	bool				value_changed;
	t_light				*light;
	char				value_str[32];
	t_ui_label			*label;

	if (button->type == UI_VALUE_BUTTON)
	{
		value_btn = (t_ui_value_button *)button->data;
		relative_x = click_x - button->pos.x;
		button_mid = button->size.x / 2.0f;
		old_value = *value_btn->value;
		value_changed = false;
		if (relative_x < button_mid)
			*value_btn->value -= value_btn->step;
		else
			*value_btn->value += value_btn->step;
		if (*value_btn->value < value_btn->range.x)
			*value_btn->value = value_btn->range.x;
		else if (*value_btn->value > value_btn->range.y)
			*value_btn->value = value_btn->range.y;
		value_changed = (*value_btn->value != old_value);
		if (value_changed)
		{
			if (button->parent && button->parent->parent &&
				button->parent->parent->type == UI_SECTION &&
				button->parent->parent->parent &&
				button->parent->parent->parent->type == UI_PANEL &&
				button->parent->parent->parent->parent &&
				button->parent->parent->parent->parent->data)
			{
				ui = (t_ui *)button->parent->parent->parent->parent->data;
				ui->needs_redraw = true;
			}
			if (button->parent && button->parent->parent &&
				button->parent->parent->data)
			{
				light = (t_light *)button->parent->parent->data;
				if (value_btn->value >= &light->color.x &&
					value_btn->value <= &light->color.z + 1)
					*value_btn->value = (int)(*value_btn->value);
			}
			if (value_btn->value_label)
			{
				snprintf(value_str, sizeof(value_str), "%.2f", *value_btn->value);
				if (value_btn->value_label->data)
				{
					label = (t_ui_label *)value_btn->value_label->data;
					free(label->text);
					label->text = ft_strdup(value_str);
					update_label_text(value_btn->value_label, value_str);
				}
			}
		}
	}
	else if (button->type == UI_BUTTON)
	{
		t_ui_button *btn = (t_ui_button *)button->data;
		if (btn->on_click)
			btn->on_click(button, btn->param);
	}
}

void decrement_value_button(t_ui_element *btn, void *param)
{
	(void)param;
	t_ui_value_button	*vb = (t_ui_value_button *)btn->data;
	char				str[32];

	*vb->value = fmaxf(vb->range.x, *vb->value - vb->step);
	snprintf(str, sizeof(str), "%.2f", *vb->value);
	update_label_text(vb->value_label, str);
}

void increment_value_button(t_ui_element *btn, void *param)
{
	(void)param;
	t_ui_value_button	*vb = (t_ui_value_button *)btn->data;
	char				str[32];

	*vb->value = fminf(vb->range.y, *vb->value + vb->step);
	snprintf(str, sizeof(str), "%.2f", *vb->value);
	update_label_text(vb->value_label, str);
}
