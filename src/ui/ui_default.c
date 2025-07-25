/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_default.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 16:25:35 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/25 18:00:47 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	default_panel(t_ui_element *panel, t_v2f pos, t_v2f size)
{
	panel->type = UI_PANEL;
	panel->pos = pos;
	panel->size = size;
	panel->visible = true;
	panel->instance_id = -1;
	panel->first_child = NULL;
	panel->next_sibling = NULL;
	panel->style = (t_ui_style){
		.bg_color = UI_PANEL_BG_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 4,
		.visible = true
	};
	
}

void	default_label(t_ui_element *label, t_v2f pos, t_v2f size)
{
	label->type = UI_LABEL;
	label->pos = pos;
	label->size = size;
	label->visible = true;
	label->instance_id = -1;
	label->first_child = NULL;
	label->next_sibling = NULL;
	label->style = (t_ui_style){
		.bg_color = UI_TRANSPARENT,
		.fg_color = UI_TRANSPARENT,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_TRANSPARENT,
		.padding = 0,
		.visible = true
	};
}

void	default_header(t_ui_element *header, t_v2f pos, t_v2f size)
{
	header-> type = UI_HEADER;
	header-> style = (t_ui_style){
		.bg_color = UI_SECTION_HEADER_COLOR,
		.fg_color = UI_TRANSPARENT,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = UI_SECTION_PADDING,
		.visible = true
	};
	header->pos = pos;
	header->size = size;
	header->visible = true;
	header->instance_id = -1;
}
