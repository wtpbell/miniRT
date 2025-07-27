/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:53:50 by bewong            #+#    #+#             */
/*   Updated: 2025/07/27 23:23:08 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdlib.h>

static void	(*const g_default_stylers[])(t_ui_element *, t_v2f, t_v2f) = {
	[UI_PANEL] = default_panel,
	[UI_BUTTON] = default_button,
	[UI_LABEL] = default_label,
	[UI_HEADER] = default_header,
	[UI_SECTION] = default_section,
};

t_ui_element	*ui_element_create(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->data = NULL;
	element->action = NULL;
	element->visible = true;
	element->image = NULL;
	element->instance_id = -1;
	if (type < (sizeof(g_default_stylers) / sizeof(g_default_stylers[0])) && 
		g_default_stylers[type] != NULL)
		g_default_stylers[type](element, pos, size);
	return (element);
}

void	layout_children(t_ui_element *parent)
{
	t_ui_element	*child;
	float			cursor_y;
	int				padding;
	mlx_instance_t	*inst;

	if (!parent)
		return ;
	child = parent->first_child;
	cursor_y = parent->style.padding;
	padding = parent->style.padding;
	while (child)
	{
		if (child->visible && child->style.visible && child->image && child->instance_id >= 0)
		{
			inst = &child->image->instances[child->instance_id];
			inst->x = (int)(parent->pos.x + padding);
			inst->y = (int)(parent->pos.y + cursor_y);
			child->pos.x = padding;
			child->pos.y = cursor_y;
			cursor_y += child->size.y + padding;
		}
		child = child->next_sibling;
	}
}

void	attach_child(t_ui_element *parent, t_ui_element *child)
{
	if (!parent || !child)
		return;
	child->parent = parent;
	child->next_sibling = parent->first_child;
	parent->first_child = child;
	layout_children(parent);
}

void	ui_images_destroy(mlx_t *mlx, t_ui_images *images)
{
	if (!images)
		return ;
	if (mlx)
	{
		if (images->button_img)
			mlx_delete_image(mlx, images->button_img);
		if (images->header_img)
			mlx_delete_image(mlx, images->header_img);
		if (images->panel_img)
			mlx_delete_image(mlx, images->panel_img);
	}
	free(images);
}
