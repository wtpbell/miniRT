/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_element_handlers.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 21:25:00 by cascade       #+#    #+#                 */
/*   Updated: 2025/07/29 16:39:33 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "libft.h"
#include "vector.h"
#include <stdlib.h>
#include <math.h>

static void	destroy_label(t_ui_element *element, mlx_t *mlx)
{
	t_ui_label	*label;

	(void)mlx;
	if (!element || !element->data)
		return ;
	label = (t_ui_label *)element->data;
	if (label->text)
		free(label->text);
	free(element->data);
	element->data = NULL;
}

static void	destroy_button(t_ui_element *element, mlx_t *mlx)
{
	t_ui_button	*button;
	
	(void)mlx;
	if (!element || !element->data)
		return ;
	button = (t_ui_button *)element->data;
	if (button->label)
		free(button->label);
	free(button);
	element->data = NULL;
}

static void	destroy_value_button(t_ui_element *element, mlx_t *mlx)
{
	t_ui_value_button	*vbutton;
	
	(void)mlx;
	if (!element || !element->data)
		return ;
	vbutton = (t_ui_value_button *)element->data;
	if (vbutton->label)
		free(vbutton->label);
	free(vbutton);
	element->data = NULL;
}
void (*const g_destroy_handlers[])(t_ui_element *, mlx_t *) = {
	[UI_PANEL] = NULL,
	[UI_BUTTON] = destroy_button,
	[UI_LABEL] = destroy_label,
	[UI_HEADER] = destroy_button,
	[UI_SECTION] = destroy_label,
	[UI_VALUE_BUTTON] = destroy_value_button,
};

void	safe_call_destroy_handler(t_ui_element *element, mlx_t *mlx)
{
	if (!element)
		return ;
	if (element->type < 0 || element->type >= (int)(sizeof(g_destroy_handlers) / sizeof(g_destroy_handlers[0])))
		return ;
	if (g_destroy_handlers[element->type] == NULL)
		return ;
	g_destroy_handlers[element->type](element, mlx);
}

static void	render_panel(t_ui_element *element, mlx_image_t *target)
{
	t_v2f abs_pos = element->pos;
	t_v2f abs_size = element->size;

	draw_rect(target, abs_pos, abs_size, element->style.bg_color);
	draw_rect(target, abs_pos, init_v2f(abs_size.x, 1), UI_BORDER_COLOR);
	draw_rect(target, abs_pos, init_v2f(1, abs_size.y), UI_BORDER_COLOR);
	draw_rect(target, init_v2f(abs_pos.x, abs_pos.y + abs_size.y - 1), 
		init_v2f(abs_size.x, 1), UI_BORDER_COLOR);
	draw_rect(target, init_v2f(abs_pos.x + abs_size.x - 1, abs_pos.y), 
		init_v2f(1, abs_size.y), UI_BORDER_COLOR);
}


static void	render_section(t_ui_element *element, mlx_image_t *target)
{
	t_v2f abs_pos = element->pos;
	t_v2f abs_size = element->size;
	t_ui_label *label = (t_ui_label *)element->data;

	draw_rect(target, abs_pos, abs_size, element->style.bg_color);
	draw_rect(target, abs_pos, 
		init_v2f(abs_size.x, UI_SECTION_HEADER_HEIGHT),
		UI_SECTION_HEADER_COLOR);
	if (label && label->text)
	{
		draw_text(target, label->text,
			init_v2f(abs_pos.x + element->style.padding,
			         abs_pos.y + (UI_SECTION_HEADER_HEIGHT - UI_CHAR_HEIGHT) / 2),
			UI_TEXT_COLOR);
	}
}
static void	render_button(t_ui_element *element, mlx_image_t *target)
{
	t_ui_button	*button = (t_ui_button *)element->data;
	t_v2f		abs_pos = element->pos;
	t_v2f		abs_size = element->size;

	if (!button)
		return ;

	draw_rect(target, abs_pos, abs_size, UI_BUTTON_COLOR);
	draw_rect(target, abs_pos, init_v2f(abs_size.x, 1), UI_BUTTON_BORDER_COLOR);
	draw_rect(target, abs_pos, init_v2f(1, abs_size.y), UI_BUTTON_BORDER_COLOR);
	draw_rect(target, init_v2f(abs_pos.x, abs_pos.y + abs_size.y - 1), 
		init_v2f(abs_size.x, 1), UI_BUTTON_BORDER_COLOR);
	draw_rect(target, init_v2f(abs_pos.x + abs_size.x - 1, abs_pos.y), 
		init_v2f(1, abs_size.y), UI_BUTTON_BORDER_COLOR);

	if (button->label)
	{
		int text_width = ft_strlen(button->label) * UI_CHAR_WIDTH;
		int text_x = abs_pos.x + (abs_size.x - text_width) / 2;
		int text_y = abs_pos.y + (abs_size.y - UI_CHAR_HEIGHT) / 2;
		draw_text(target, button->label, 
			init_v2f(text_x, text_y), UI_TEXT_COLOR);
	}
}


void ui_element_setup_handlers(t_ui_element *element)
{
	if (!element)
		return ;
	printf("DEBUG: Setting up handlers for element type %d at %p\n", 
		element->type, (void *)element);
	element->render = NULL;
	element->destroy = NULL;
	element->update = NULL;
	if (element->type == UI_PANEL)
		element->render = render_panel;
	else if (element->type == UI_SECTION)
		element->render = render_section;
	else if (element->type == UI_BUTTON)
		element->render = render_button;
	else if (element->type == UI_VALUE_BUTTON)
		element->render = render_button;
	printf("DEBUG: Handlers setup complete for type %d\n", element->type);
}
