/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_styles.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 16:25:35 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/29 09:11:37 by bewong        ########   odam.nl         */
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
		.fg_color = UI_PANEL_BG_COLOR,
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
	label->instance_id = -1;
	label->first_child = NULL;
	label->next_sibling = NULL;
	label->style = (t_ui_style){
		.bg_color = UI_TRANSPARENT,
		.fg_color = UI_TRANSPARENT,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_TRANSPARENT,
		.padding = 0,
		.visible = true,
	};
}

void	default_header(t_ui_element *header, t_v2f pos, t_v2f size)
{
	header-> type = UI_HEADER;
	header->pos = pos;
	header->size = size;
	header->visible = true;
	header->instance_id = -1;
	header->first_child = NULL;
	header->next_sibling = NULL;
	header-> style = (t_ui_style){
		.bg_color = UI_SECTION_HEADER_COLOR,
		.fg_color = UI_TRANSPARENT,
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
	button->instance_id = -1;
	button->first_child = NULL;
	button->next_sibling = NULL;
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.fg_color = UI_BUTTON_HOVER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 4,
		.visible = true,
	};
}

void	default_value_button(t_ui_element *button, t_v2f pos, t_v2f size, const char *label)
{
	t_ui_button	*btn_data;

	button->type = UI_VALUE_BUTTON;
	button->pos = pos;
	button->size = size;
	button->visible = true;
	button->instance_id = -1;
	button->first_child = NULL;
	button->next_sibling = NULL;
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.fg_color = UI_BUTTON_HOVER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 4,
		.visible = true,
	};
	if (label && *label) 
	{
		// [-] | [+]
		btn_data = (t_ui_button *)button->data;
		if (!btn_data) {
			btn_data = (t_ui_button *)ft_calloc(1, sizeof(t_ui_button));
			if (btn_data)
			{
				btn_data->label = ft_strdup(label);
				button->data = btn_data;
			}
		} else if (btn_data->label)
		{
			free(btn_data->label);
			btn_data->label = ft_strdup(label);
		}
	}
}

void	default_section(t_ui_element *section, t_v2f pos, t_v2f size)
{
	section->type = UI_SECTION;
	section->pos = pos;
	section->size = size;
	section->visible = true;
	section->instance_id = -1;
	section->first_child = NULL;
	section->next_sibling = NULL;
	section->style = (t_ui_style){
		.bg_color = UI_SECTION_COLOR,
		.fg_color = UI_SECTION_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = UI_SECTION_PADDING,
		.visible = true,
	};
}
