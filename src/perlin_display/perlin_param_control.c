/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_param_control.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:25:02 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:25:02 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"
#include "ui.h"
#include "rt_snprintf.h"

static void	format_param_value(char *dst, size_t size, int i)
{
	if (i == 9)
		rt_snprintf(dst, size, "%d", *(int *)g_params[i].value);
	else
		rt_snprintf(dst, size, "%f", *g_params[i].value);
}

static void	row_style(t_ui_element *row, int i)
{
	if (i == 0)
	{
		row->style.bg_color = UI_ACTIVE;
		row->style.border_color = UI_ACTIVE_BORDER;
	}
	else
	{
		row->style.bg_color = UI_PANEL_BG_COLOR;
		row->style.border_color = UI_TRANSPARENT;
	}
}

static void	create_param_row(t_ui *ui, t_ui_element *parent,
			t_pdisplay *display, int i)
{
	t_ui_element	*row;
	t_ui_element	*label;
	char			value_str[32];

	row = create_panel(ui->context,
			init_v2f(0, UI_HEADER_HEIGHT + (i * UI_ROW_HEIGHT)),
			init_v2f(UI_PANEL_WIDTH, UI_ROW_HEIGHT));
	row_style(row, i);
	attach_child(parent, row);
	label = create_label(ui->context, g_params[i].name,
			init_v2f(UI_LABEL_PADDING * 4,
				(UI_ROW_HEIGHT - UI_FONT_HEIGHT) / 2), UI_TEXT_COLOR);
	attach_child(row, label);
	format_param_value(value_str, sizeof(value_str), i);
	label = create_label(ui->context, value_str,
			init_v2f(UI_LABEL_WIDTH + UI_LABEL_PADDING,
				(UI_ROW_HEIGHT - UI_FONT_HEIGHT) / 2), UI_TEXT_COLOR);
	attach_child(row, label);
	display->params[i].value = g_params[i].value;
	display->params[i].range = g_params[i].range;
	display->params[i].label = (t_ui_label *)label->data;
	display->params[i].row = row;
}

void	add_parameter_controls(t_ui *ui, t_ui_element *parent,
		t_pdisplay *display)
{
	int	i;

	display->param_count = sizeof(g_params) / sizeof(g_params[0]);
	i = 0;
	while (i < display->param_count)
	{
		create_param_row(ui, parent, display, i);
		i++;
	}
}
