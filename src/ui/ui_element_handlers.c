/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_element_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:25:00 by bewong           #+#    #+#             */
/*   Updated: 2025/08/04 16:13:11 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	destroy_label(t_ui_element *element)
{
	t_ui_label	*label;

	if (!element || !element->data)
		return ;
	label = (t_ui_label *)element->data;
	if (label->text)
		free(label->text);
	free(element->data);
	element->data = NULL;
}

static void	destroy_button(t_ui_element *element)
{
	t_ui_btn	*btn;

	if (!element || !element->data)
		return ;
	btn = (t_ui_btn *)element->data;
	if (btn->label)
	{
		free(btn->label);
		btn->label = NULL;
	}
	free(element->data);
	element->data = NULL;
}

static void	destroy_value_button(t_ui_element *element)
{
	t_ui_vbtn	*vbutton;

	if (!element || !element->data)
		return ;
	vbutton = (t_ui_vbtn *)element->data;
	if (vbutton->label)
		free(vbutton->label);
	free(vbutton);
	element->data = NULL;
}

void (*const	g_destroy_handlers[])(t_ui_element *) = {
[UI_PANEL] = NULL,
[UI_BUTTON] = destroy_button,
[UI_LABEL] = destroy_label,
[UI_HEADER] = NULL,
[UI_SECTION] = NULL,
[UI_VALUE_BUTTON] = destroy_value_button
};

void	destroy_ui_element(t_ui_element *element)
{
	if (!element)
		return ;
	if (element->type >= 0 && element->type < (int)(sizeof(g_destroy_handlers)
		/ sizeof(g_destroy_handlers[0])) &&
		g_destroy_handlers[element->type] != NULL)
		g_destroy_handlers[element->type](element);
	else if (element->data)
	{
		free(element->data);
		element->data = NULL;
	}
	free(element);
}
