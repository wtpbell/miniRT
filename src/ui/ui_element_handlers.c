/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_element_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 21:25:00 by cascade           #+#    #+#             */
/*   Updated: 2025/07/31 00:16:33 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "libft.h"
#include "vector.h"
#include <stdlib.h>
#include <math.h>

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
	t_ui_button	*button;
	
	(void)ctx;
	if (!element || !element->data)
		return ;
	button = (t_ui_button *)element->data;
	if (button->label)
		free(button->label);
	free(button);
	element->data = NULL;
}

static void	destroy_value_button(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_value_button	*vbutton;
	
	(void)ctx;
	if (!element || !element->data)
		return ;
	vbutton = (t_ui_value_button *)element->data;
	if (vbutton->label)
		free(vbutton->label);
	free(vbutton);
	element->data = NULL;
}

void (*const g_destroy_handlers[])(t_ui_element *, t_ui_context *) = {
	[UI_PANEL] = NULL,
	[UI_BUTTON] = destroy_button,
	[UI_LABEL] = destroy_label,
	[UI_HEADER] = destroy_button,
	[UI_SECTION] = destroy_label,
	[UI_VALUE_BUTTON] = destroy_value_button,
};

void	safe_call_destroy_handler(t_ui_element *element, t_ui_context *ctx)
{
	if (!element || !ctx)
		return;
	if (element->type < 0 || element->type >= (int)(sizeof(g_destroy_handlers) / sizeof(g_destroy_handlers[0])))
		return;
	if (g_destroy_handlers[element->type] == NULL)
		return;
	g_destroy_handlers[element->type](element, ctx);
}

void	destroy_ui_element(t_ui_element *element, t_ui_context *ctx, bool free_data)
{
	if (!element || !ctx)
		return;

	safe_call_destroy_handler(element, ctx);
	if (free_data && element->data)
	{
		free(element->data);
		element->data = NULL;
	}
	free(element);
}

static void	render_panel(t_ui_element *element, t_ui_context *ctx)
{
	t_v2f abs_pos = element->pos;
	t_v2f abs_size = element->size;

	if (!ctx || !ctx->canvas)
		return;

	draw_rect(ctx->canvas, abs_pos, abs_size, element->style.bg_color);
	draw_rect_border(ctx->canvas, abs_pos, abs_size, element->style.border_color);
}

t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;

	(void)ctx;

	panel = ui_element_create(UI_PANEL, pos, size);
	if (!panel)
		return (NULL);
	default_panel(panel, pos, size);
	return (panel);
}

t_ui_element	*create_button(t_ui_context *ctx, const char *label_text, t_v2f pos, t_v2f size,
	void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element	*button;
	t_ui_button	*btn_data;

	(void)ctx;

	button = ui_element_create(UI_BUTTON, pos, size);
	if (!button)
		return (NULL);
	default_button(button, pos, size);
	button->action = on_click;
	button->data = ft_calloc(1, sizeof(t_ui_button));
	if (!button->data)
	{
		free(button);
		return (NULL);
	}
	btn_data = (t_ui_button *)button->data;
	btn_data->on_click = on_click;
	btn_data->param = param;
	btn_data->label = ft_strdup(label_text);
	if (!btn_data->label)
	{
		free(button->data);
		free(button);
		return (NULL);
	}
	return (button);
}

t_ui_element	*create_label(t_ui_context *ctx, const char *text, t_v2f pos, uint32_t color)
{
	t_ui_element	*label_elem;
	t_ui_label	*label_data;

	(void)ctx;
	label_elem = ui_element_create(UI_LABEL, pos, init_v2f(0, 0));
	if (!label_elem)
		return (NULL);
	default_label(label_elem, pos, init_v2f(0, 0));
	label_elem->style.text_color = color;
	label_elem->data = ft_calloc(1, sizeof(t_ui_label));
	if (!label_elem->data)
	{
		free(label_elem);
		return (NULL);
	}
	label_data = (t_ui_label *)label_elem->data;
	label_data->text = ft_strdup(text);
	if (!label_data->text)
	{
		free(label_elem->data);
		free(label_elem);
		return (NULL);
	}
	label_elem->size.x = ft_strlen(text) * UI_CHAR_WIDTH;
	label_elem->size.y = UI_CHAR_HEIGHT;
	return (label_elem);
}

static void	render_section(t_ui_element *element, t_ui_context *ctx)
{
	t_v2f abs_pos = element->pos;
	t_v2f abs_size = element->size;
	t_ui_label *label = (t_ui_label *)element->data;
	
	if (!ctx || !ctx->canvas)
		return;

	draw_rect(ctx->canvas, abs_pos, abs_size, element->style.bg_color);
	draw_rect(ctx->canvas, abs_pos, 
		init_v2f(abs_size.x, UI_SECTION_HEADER_HEIGHT),
		UI_SECTION_HEADER_COLOR);
	if (label && label->text)
	{
		draw_text(ctx->canvas, label->text,
			init_v2f(abs_pos.x + element->style.padding,
			         abs_pos.y + (UI_SECTION_HEADER_HEIGHT - UI_CHAR_HEIGHT) / 2),
			UI_TEXT_COLOR);
	}
}

static void	render_button(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_button	*button = (t_ui_button *)element->data;
	t_v2f		abs_pos = element->pos;
	t_v2f		abs_size = element->size;

	if (!button || !ctx || !ctx->canvas)
		return ;

	draw_rect(ctx->canvas, abs_pos, abs_size, UI_BUTTON_COLOR);
	draw_rect(ctx->canvas, abs_pos, init_v2f(abs_size.x, 1), UI_BUTTON_BORDER_COLOR);
	draw_rect(ctx->canvas, abs_pos, init_v2f(1, abs_size.y), UI_BUTTON_BORDER_COLOR);
	draw_rect(ctx->canvas, init_v2f(abs_pos.x, abs_pos.y + abs_size.y - 1), 
		init_v2f(abs_size.x, 1), UI_BUTTON_BORDER_COLOR);
	draw_rect(ctx->canvas, init_v2f(abs_pos.x + abs_size.x - 1, abs_pos.y), 
		init_v2f(1, abs_size.y), UI_BUTTON_BORDER_COLOR);

	if (button->label)
	{
		int text_width = ft_strlen(button->label) * UI_CHAR_WIDTH;
		int text_x = abs_pos.x + (abs_size.x - text_width) / 2;
		int text_y = abs_pos.y + (abs_size.y - UI_CHAR_HEIGHT) / 2;
		draw_text(ctx->canvas, button->label, 
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
