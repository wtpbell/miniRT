/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:51:30 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 18:08:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ui.h"

#define VALUE_STR_LEN 32

static bool	is_point_in_element(const t_ui_element *element,
				int32_t x, int32_t y)
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
	return (x >= abs_pos.x && x <= (abs_pos.x + element->size.x)
		&& y >= abs_pos.y && y <= (abs_pos.y + element->size.y));
}

void	update_value_label(t_ui_vbtn *btn, t_ui_context *ctx)
{
	const char	*value_str;
	t_ui_label	*label_data;
	static char	fallback_buf[VALUE_STR_LEN];

	if (!btn || !ctx)
		return ;
	if (btn->formatter)
		value_str = btn->formatter(*btn->value);
	else
	{
		snprintf(fallback_buf, sizeof(fallback_buf), "%.2f", *btn->value);
		value_str = fallback_buf;
	}
	label_data = (t_ui_label *)btn->value_label->data;
	if (label_data && label_data->text)
	{
		free(label_data->text);
		label_data->text = ft_strdup(value_str);
		ui_mark_dirty(ctx);
	}
}

void	handle_ui_click(t_ui_element *root, int32_t x,
				int32_t y, t_ui_context *ctx)
{
	t_ui_element	*child;
	t_ui_btn		*btn;

	if (is_point_in_element(root, x, y))
	{
		if ((root->type == UI_BUTTON || root->type == UI_VALUE_BUTTON)
			&& root->data)
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

void	update_value_button(t_ui_element *button, float new_value,
							t_ui_context *ctx)
{
	t_ui_vbtn	*btn;

	if (!button || button->type != UI_VALUE_BUTTON || !ctx)
		return ;
	btn = (t_ui_vbtn *)button->data;
	if (btn && btn->value)
	{
		if (new_value < btn->range.x)
			new_value = btn->range.x;
		else if (new_value > btn->range.y)
			new_value = btn->range.y;
		if (*btn->value != new_value)
		{
			*btn->value = new_value;
			if (btn->on_click)
				btn->on_click(button, btn->param);
			update_value_label(btn, ctx);
			ui_mark_dirty(ctx);
		}
	}
}

static void	update_value(t_ui_vbtn *v, float new_val,
				t_ui_element *b, t_ui_context *c)
{
	if (new_val < v->range.x)
		new_val = v->range.x;
	else if (new_val > v->range.y)
		new_val = v->range.y;
	if (new_val != *v->value)
		update_value_button(b, new_val, c);
}

void	update_button_value(t_ui_element *b, int32_t x, t_ui_context *c)
{
	t_v2f			pos;
	t_ui_vbtn		*v;
	t_ui_element	*p;

	pos = b->pos;
	p = b->parent;
	while (p)
	{
		pos.x += p->pos.x;
		pos.y += p->pos.y;
		p = p->parent;
	}
	v = (t_ui_vbtn *)b->data;
	if (!v->value)
		return ;
	if (x - pos.x < b->size.x / 2.0f)
		update_value(v, *v->value - v->step, b, c);
	else
		update_value(v, *v->value + v->step, b, c);
}

void	increment_value_button(t_ui_element *btn, void *param)
{
	t_ui_context	*ctx;
	t_ui_vbtn		*btn_data;

	if (!btn || btn->type != UI_VALUE_BUTTON || !param)
		return ;
	ctx = (t_ui_context *)param;
	btn_data = (t_ui_vbtn *)btn->data;
	if (btn_data && btn_data->value)
	{
		*btn_data->value += btn_data->step;
		if (*btn_data->value > btn_data->range.y)
			*btn_data->value = btn_data->range.y;
		update_value_label(btn_data, ctx);
		if (btn_data->on_click)
			btn_data->on_click(btn, btn_data->param);
		ui_mark_dirty(ctx);
	}
}

void	decrement_value_button(t_ui_element *btn, void *param)
{
	t_ui_context	*ctx;
	t_ui_vbtn		*btn_data;

	if (!btn || btn->type != UI_VALUE_BUTTON || !param)
		return ;
	ctx = (t_ui_context *)param;
	btn_data = (t_ui_vbtn *)btn->data;
	if (btn_data && btn_data->value)
	{
		*btn_data->value -= btn_data->step;
		if (*btn_data->value < btn_data->range.x)
			*btn_data->value = btn_data->range.x;
		update_value_label(btn_data, ctx);
		if (btn_data->on_click)
			btn_data->on_click(btn, btn_data->param);
		ui_mark_dirty(ctx);
	}
}
