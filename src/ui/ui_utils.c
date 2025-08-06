/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:38:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 19:42:31 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char	*ft_ftoa(float f, int precision)
{
	int		int_part;
	float	decimal;
	int		decimal_part;
	char	*str;

	if (precision < 0)
		precision = 0;
	int_part = (int)f;
	decimal = f - int_part;
	decimal_part = (int)(decimal * pow(10, precision) + 0.5f);
	str = (char *)calloc(32, sizeof(char));
	if (!str)
		return (NULL);
	if (precision > 0)
		snprintf(str, 32, "%d.%0*d", int_part, precision, decimal_part);
	else
		snprintf(str, 32, "%d", int_part);
	return (str);
}

char	*format_float_value(float value)
{
	static char	buf[16];

	snprintf(buf, sizeof(buf), "%.2f", value);
    return (buf);
}

char	*format_color_value(float value)
{
	static char	buf[16];
	
	snprintf(buf, sizeof(buf), "%6.2f", value * 255.0f);
	return (buf);
}

t_light	*find_light(t_scene *scene, t_light_type type)
{
	t_light	*light;
	int		i;

	i = 0;
	while (i < (int)scene->lights.size)
	{
		light = (t_light *)vector_get(&scene->lights, i);
		if (light->type == type)
			return light;
		i++;
	}
	return NULL;
}

// Forward declaration
void ui_element_setup_handlers(t_ui_element *element);

bool	ui_element_remove_child(t_ui_element *parent, t_ui_element *child, 
							bool destroy, t_ui_context *ctx)
{
	t_ui_element	*current;
	t_ui_element	*prev;

	if (!parent || !child || child->parent != parent || !ctx)
		return (false);
	prev = NULL;
	current = parent->first_child;
	while (current && current != child)
	{
		prev = current;
		current = current->next_sibling;
	}
	if (!current)
		return (false);
	if (prev)
		prev->next_sibling = child->next_sibling;
	else
		parent->first_child = child->next_sibling;
	child->parent = NULL;
	child->next_sibling = NULL;
	if (destroy)
		destroy_ui_element_recursive(child, ctx, true);
	return (true);
}

void	destroy_ui_element_recursive(t_ui_element *element, t_ui_context *ctx, bool free_data)
{
	t_ui_element	*child;
	t_ui_element	*next;

	if (!element || !ctx)
		return ;
	child = element->first_child;
	while (child)
	{
		next = child->next_sibling;
		ui_element_remove_child(element, child, true, ctx);
		child = next;
	}
	if (element->parent)
		ui_element_remove_child(element->parent, element, false, ctx);
	destroy_ui_element(element, ctx, free_data);
}

t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = ft_calloc(1, sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->visible = true;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->parent = NULL;
	element->data = NULL;
	element->action = NULL;
	element->render = NULL;
	element->state = 0;
	element->abs_pos = init_v2f(0, 0);
	element->style = (t_ui_style){
		.bg_color = 0x00000000,
		.border_color = 0x00000000,
		.text_color = 0xFFFFFFFF,
		.padding = 0,
		.visible = true
	};
	// Set up the element's handlers based on its type
	ui_element_setup_handlers(element);
	return (element);
}

void	layout_vertical(t_ui_element *parent, float spacing)
{
	t_ui_element	*child;
	t_v2f			pos;

	if (!parent || !parent->first_child)
		return ;
	pos = parent->pos;
	pos.x += parent->style.padding;
	pos.y += parent->style.padding;
	child = parent->first_child;
	while (child)
	{
		if (child->visible)
		{
			child->pos = pos;
			pos.y += child->size.y + spacing;
		}
		child = child->next_sibling;
	}
}

void	layout_horizontal(t_ui_element *parent, float spacing)
{
	t_ui_element	*child;
	t_v2f			pos;

	if (!parent || !parent->first_child)
		return ;
	pos = parent->pos;
	pos.x += parent->style.padding;
	pos.y += parent->style.padding;

	child = parent->first_child;
	while (child)
	{
		if (child->visible)
		{
			child->pos = pos;
			pos.x += child->size.x + spacing;
		}
		child = child->next_sibling;
	}
}
