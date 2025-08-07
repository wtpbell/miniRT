/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 22:38:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/07 11:23:21 by bewong        ########   odam.nl         */
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
		destroy_ui_element_recursive(child, ctx);
	return (true);
}

void	destroy_ui_element_recursive(t_ui_element *element, t_ui_context *ctx)
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
	destroy_ui_element(element, ctx);
}
