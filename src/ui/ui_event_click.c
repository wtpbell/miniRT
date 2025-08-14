/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_event_click.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:30:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/14 22:00:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

#define VALUE_STR_LEN 32

void	update_value_label(t_ui_vbtn *btn, t_ui_context *ctx)
{
	const char	*value_str;
	t_ui_label	*label_data;
	static char	fallback_buf[VALUE_STR_LEN];
	char		*new_text;

	if (btn->formatter)
		value_str = btn->formatter(*btn->value);
	else
	{
		rt_snprintf(fallback_buf, sizeof(fallback_buf), "%f", *btn->value);
		value_str = fallback_buf;
	}
	label_data = (t_ui_label *)btn->value_label->data;
	if (!label_data)
		return ;
	new_text = ft_strdup(value_str);
	if (!new_text)
		return ;
	if (label_data->text)
		free(label_data->text);
	label_data->text = new_text;
	ui_mark_dirty(ctx);
}

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

void	render_button_clicked(t_ui_element *button, void *param)
{
	t_ui_context	*ctx;
	t_game			*game;

	(void)button;
	if (!param)
		return ;
	ctx = (t_ui_context *)param;
	game = (t_game *)ctx->game;
	if (game)
	{
		game->needs_redraw = true;
		update_camera_view(&game->scene->camera);
		ui_mark_dirty(ctx);
		printf("Re-render triggered!\n");
	}
}

void	toggle_ui_visibility(t_ui *ui)
{
	size_t			i;
	t_ui_context	*ctx;

	ctx = ui->context;
	ctx->is_visible = !ctx->is_visible;
	i = 0;
	while (i < ctx->canvas->count)
	{
		ctx->canvas->instances[i].enabled = ctx->is_visible;
		i++;
	}
}
