/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_event.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 12:51:30 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/08 17:33:49 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ui.h"

#define VALUE_STR_LEN 32

static bool	is_point_in_element(const t_ui_element *element, int32_t x, int32_t y)
{
	t_v2f				abs_pos;
	const t_ui_element	*parent;

	if (!element)
		return (false);
	abs_pos = element->pos;
	parent = element->parent;
	while (parent)
	{
		abs_pos.x += parent->pos.x;
		abs_pos.y += parent->pos.y;
		parent = parent->parent;
	}
	return (x >= abs_pos.x &&
			x <= (abs_pos.x + element->size.x) &&
			y >= abs_pos.y &&
			y <= (abs_pos.y + element->size.y));
}

static void	update_value_label(t_ui_vbtn *btn, t_ui_context *ctx)
{
	const char	*value_str;
	t_ui_label	*label_data;
	static char	fallback_buf[VALUE_STR_LEN];

	if (btn->formatter)
		value_str = btn->formatter(*btn->value);
	else
	{
		snprintf(fallback_buf, sizeof(fallback_buf), "%.2f", *btn->value);
		value_str = fallback_buf;
	}
	if (btn->value_label->type == UI_LABEL)
	{
		label_data = (t_ui_label *)btn->value_label->data;
		if (label_data)
		{
			if (label_data->text)
				free(label_data->text);
			label_data->text = ft_strdup(value_str);
			ctx->needs_redraw = true;
		}
	}
}

void	handle_ui_click(t_ui_element *root, int32_t x, int32_t y, t_ui_context *ctx)
{
	t_ui_element	*child;
	t_ui_btn		*btn;

	if (is_point_in_element(root, x, y))
	{
		if ((root->type == UI_BUTTON || root->type == UI_VALUE_BUTTON) && root->data)
		{
			if (root->type == UI_VALUE_BUTTON)
				update_button_value(root, x, ctx);
			else
			{
				btn = (t_ui_btn *)root->data;
				if (btn->on_click)
					btn->on_click(root, btn->param);
			}
			return ;
		}
	}
	child = root->first_child;
	while (child)
	{
		handle_ui_click(child, x, y, ctx);
		child = child->next_sibling;
	}
}

static void	update_value_button(t_ui_element *button, float new_value,
							t_ui_context *ctx)
{
	t_ui_vbtn	*value_btn;

	if (!button || !button->data || !ctx)
		return ;
	value_btn = (t_ui_vbtn *)button->data;
	*value_btn->value = new_value;
	if (*value_btn->value < value_btn->range.x)
		*value_btn->value = value_btn->range.x;
	else if (*value_btn->value > value_btn->range.y)
		*value_btn->value = value_btn->range.y;
	update_value_label(value_btn, ctx);
	if (button->action)
		button->action(button, ctx);
}

void	update_button_value(t_ui_element *button, int32_t click_x, t_ui_context *ctx)
{
	const t_ui_element	*parent = button->parent;
	t_ui_vbtn			*value_btn;
	float				relative_x;
	float				button_mid;
	float				old_value;
	t_v2f				abs_pos;
	float				new_value;

	abs_pos = button->pos;
	while (parent)
	{
		abs_pos.x += parent->pos.x;
		abs_pos.y += parent->pos.y;
		parent = parent->parent;
	}
	value_btn = (t_ui_vbtn *)button->data;
	relative_x = click_x - abs_pos.x;
	button_mid = button->size.x / 2.0f;
	old_value = *value_btn->value;
	new_value = old_value + 
					((relative_x < button_mid) ? -value_btn->step : value_btn->step);
	if (new_value != old_value)
		update_value_button(button, new_value, ctx);
}

void	increment_value_button(t_ui_element *btn, void *param)
{
	t_ui_vbtn		*vb;
	t_ui_context	*ctx;
	float			new_value;

	if (!btn || btn->type != UI_VALUE_BUTTON || !btn->data || !param)
		return ;
	vb = (t_ui_vbtn *)btn->data;
	ctx = (t_ui_context *)param;
	new_value = *vb->value + vb->step;
	if (new_value > vb->range.y)
		new_value = vb->range.y;
	if (new_value != *vb->value)
		update_value_button(btn, new_value, ctx);
}

void	decrement_value_button(t_ui_element *btn, void *param)
{
	t_ui_vbtn		*vb;
	t_ui_context	*ctx;
	float			new_value;

	if (!btn || btn->type != UI_VALUE_BUTTON || !btn->data || !param)
		return;
	vb = (t_ui_vbtn *)btn->data;
	ctx = (t_ui_context *)param;
	new_value = *vb->value - vb->step;
	if (new_value < vb->range.x)
		new_value = vb->range.x;
	if (new_value != *vb->value)
		update_value_button(btn, new_value, ctx);
}
