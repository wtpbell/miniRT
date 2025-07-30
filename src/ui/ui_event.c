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

void handle_ui_click(t_ui_element *root, int32_t x, int32_t y, t_ui_context *ctx)
{
	t_ui_element *child;

	if (!root || !ctx || !is_point_in_element(root, x, y))
		return;

	if (root->type == UI_BUTTON && root->data)
		update_button_value(root, x, ctx);

	child = root->first_child;
	while (child)
	{
		handle_ui_click(child, x, y, ctx);
		child = child->next_sibling;
	}
}

void update_label_text(t_ui_element *label, const char *text, t_ui_context *ctx)
{
	if (!label || !text || !ctx || !ctx->mlx)
		return;

	// Get the label data
	t_ui_button *btn_data = (t_ui_button *)label->data;
	if (!btn_data)
		return;

	// Calculate size based on text length
	t_v2f size = init_v2f(ft_strlen(text) * UI_CHAR_WIDTH + 10, UI_CHAR_HEIGHT + 10);
	
	// Create a new image for the label text
	mlx_image_t *img = mlx_new_image(ctx->mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!img)
		return;

	// Draw the text on the image
	draw_text(img, text, init_v2f(5, 5), label->style.text_color);

	// Update the button's label
	if (btn_data->label)
		free(btn_data->label);
	btn_data->label = ft_strdup(text);

	// Store the image in the element's data
	label->data = btn_data;
}

void update_button_value(t_ui_element *button, int32_t click_x, t_ui_context *ctx)
{
	t_ui_value_button *value_btn;
	float relative_x;
	float button_mid;
	float old_value;
	bool value_changed;

	if (!button || button->type != UI_VALUE_BUTTON || !button->data || !ctx)
		return;

	value_btn = (t_ui_value_button *)button->data;
	relative_x = click_x - button->pos.x;
	button_mid = button->size.x / 2.0f;
	old_value = *value_btn->value;

	// Update value based on click position
	if (relative_x < button_mid)
		*value_btn->value -= value_btn->step;
	else
		*value_btn->value += value_btn->step;

	// Clamp value to range
	if (*value_btn->value < value_btn->range.x)
		*value_btn->value = value_btn->range.x;
	else if (*value_btn->value > value_btn->range.y)
		*value_btn->value = value_btn->range.y;

	value_changed = (*value_btn->value != old_value);

	if (value_changed)
	{
		// Update the value label if it exists
		if (value_btn->value_label)
		{
			char value_str[32];
			snprintf(value_str, sizeof(value_str), "%.2f", *value_btn->value);
			update_label_text(value_btn->value_label, value_str, ctx);
		}

		// Mark UI for redraw
		ctx->needs_redraw = true;

		// Call the button's click handler if it exists
		if (button->action)
			button->action(button, ctx);
		{
			t_light *light = (t_light *)button->parent->parent->data;
			if (value_btn->value >= &light->color.x &&
				value_btn->value <= &light->color.z + 1)
				*value_btn->value = (int)(*value_btn->value);
			if (value_btn->value_label)
			{
				char value_str[32];
				snprintf(value_str, sizeof(value_str), "%.2f", *value_btn->value);
				update_label_text(value_btn->value_label, value_str, ctx);
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
	t_ui_value_button	*vb;
	t_ui_context		*ctx;
	char			str[32];

	if (!btn || btn->type != UI_VALUE_BUTTON || !btn->data || !param)
		return ;
	
	vb = (t_ui_value_button *)btn->data;
	ctx = (t_ui_context *)param;
	
	*vb->value -= vb->step;
	if (*vb->value < vb->range.x)
		*vb->value = vb->range.x;
	
	snprintf(str, sizeof(str), "%.2f", *vb->value);
	if (vb->value_label && ctx)
		update_label_text(vb->value_label, str, ctx);
}

void increment_value_button(t_ui_element *btn, void *param)
{
	t_ui_value_button	*vb;
	t_ui_context		*ctx;
	char			str[32];

	if (!btn || btn->type != UI_VALUE_BUTTON || !btn->data || !param)
		return ;
	
	vb = (t_ui_value_button *)btn->data;
	ctx = (t_ui_context *)param;
	
	*vb->value += vb->step;
	if (*vb->value > vb->range.y)
		*vb->value = vb->range.y;
	
	snprintf(str, sizeof(str), "%.2f", *vb->value);
	if (vb->value_label && ctx)
		update_label_text(vb->value_label, str, ctx);
}
