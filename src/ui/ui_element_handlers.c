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
#include "libft.h"
#include <stdlib.h>

static void	destroy_label(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_label	*label;

	(void)ctx;
	if (!element || !element->data)
		return ;
	label = (t_ui_label *)element->data;
	if (label->text)
		free(label->text);
	free(element->data);
	element->data = NULL;
}

static void	destroy_button(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_btn	*button;
	
	(void)ctx;
	if (!element || !element->data)
		return ;
	button = (t_ui_btn *)element->data;
	if (button->label)
		free(button->label);
	free(button);
	element->data = NULL;
}

static void	destroy_value_button(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_vbtn	*vbutton;
	
	(void)ctx;
	if (!element || !element->data)
		return ;
	vbutton = (t_ui_vbtn *)element->data;
	if (vbutton->label)
		free(vbutton->label);
	free(vbutton);
	element->data = NULL;
}

void (*const g_destroy_handlers[])(t_ui_element *, t_ui_context *) = {
	[UI_PANEL] = NULL,
	[UI_BUTTON] = destroy_button,
	[UI_LABEL] = destroy_label,
	[UI_HEADER] = NULL,
	[UI_SECTION] = NULL,
	[UI_VALUE_BUTTON] = destroy_value_button
};

static void	safe_call_destroy_handler(t_ui_element *element, t_ui_context *ctx)
{
	if (!element || !ctx)
		return;
	if (element->type < 0 || element->type >= (int)(sizeof(g_destroy_handlers) / sizeof(g_destroy_handlers[0])))
		return;
	if (g_destroy_handlers[element->type] == NULL)
		return;
	g_destroy_handlers[element->type](element, ctx);
}

void	destroy_ui_element(t_ui_element *element, t_ui_context *ctx)
{
	if (!element || !ctx)
		return ;
	safe_call_destroy_handler(element, ctx);
	free(element);
}
