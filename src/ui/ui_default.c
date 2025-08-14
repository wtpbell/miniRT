/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_default.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:25:35 by bewong            #+#    #+#             */
/*   Updated: 2025/08/14 21:27:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	default_panel(t_ui_element *panel, t_v2f pos, t_v2f size)
{
	panel->type = UI_PANEL;
	panel->pos = pos;
	panel->size = size;
	panel->visible = true;
	panel->first_child = NULL;
	panel->next_sibling = NULL;
	panel->parent = NULL;
	panel->data = NULL;
	panel->action = NULL;
	panel->abs_pos = g_v2f_zero;
	panel->style = (t_ui_style){
		.bg_color = UI_PANEL_BG_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = UI_PANEL_PADDING,
		.visible = true,
	};
}

void	default_label(t_ui_element *label, t_v2f pos, t_v2f size)
{
	label->type = UI_LABEL;
	label->pos = pos;
	label->size = size;
	label->visible = true;
	label->first_child = NULL;
	label->next_sibling = NULL;
	label->parent = NULL;
	label->data = NULL;
	label->action = NULL;
	label->visible = true;
	label->abs_pos = g_v2f_zero;
	label->style = (t_ui_style){
		.bg_color = UI_TRANSPARENT,
		.border_color = UI_TRANSPARENT,
		.text_color = UI_TEXT_COLOR,
		.padding = UI_PANEL_PADDING,
		.visible = true,
	};
}

void	default_header(t_ui_element *header, t_v2f pos, t_v2f size)
{
	header->type = UI_HEADER;
	header->pos = pos;
	header->size = size;
	header->visible = true;
	header->first_child = NULL;
	header->next_sibling = NULL;
	header->parent = NULL;
	header->data = NULL;
	header->action = NULL;
	header->visible = true;
	header->abs_pos = g_v2f_zero;
	header->style = (t_ui_style){
		.bg_color = UI_HEADER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = UI_SECTION_PADDING,
		.visible = true
	};
}

void	default_button(t_ui_element *button, t_v2f pos, t_v2f size)
{
	button->type = UI_BUTTON;
	button->pos = pos;
	button->size = size;
	button->visible = true;
	button->first_child = NULL;
	button->next_sibling = NULL;
	button->parent = NULL;
	button->data = NULL;
	button->action = NULL;
	button->abs_pos = g_v2f_zero;
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BUTTON_BORDER_COLOR,
		.padding = 4,
		.visible = true,
	};
}

void	default_section(t_ui_element *section, t_v2f pos, t_v2f size)
{
	section->type = UI_SECTION;
	section->pos = pos;
	section->size = size;
	section->visible = true;
	section->first_child = NULL;
	section->next_sibling = NULL;
	section->parent = NULL;
	section->data = NULL;
	section->action = NULL;
	section->abs_pos = g_v2f_zero;
	section->style = (t_ui_style){
		.bg_color = UI_SECTION_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = UI_SECTION_PADDING,
		.visible = true,
	};
}
