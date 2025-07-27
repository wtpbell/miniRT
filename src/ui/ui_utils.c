/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:38:00 by bewong            #+#    #+#             */
/*   Updated: 2025/07/27 23:26:26 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	ui_element_destroy(t_ui_element *element, mlx_t *mlx, bool free_data)
{
	if (!element)
		return ;
	if (free_data && element->data)
	{
		safe_call_destroy_handler(element, mlx);
		element->data = NULL;
	}
	if (element->image && mlx)
	{
		if (mlx->window)
			mlx_delete_image(mlx, element->image);
		element->image = NULL;
	}
	free(element);
}

bool	ui_element_remove_child(t_ui_element *parent, t_ui_element *child, 
							bool destroy, mlx_t *mlx)
{
	t_ui_element	*current;
	t_ui_element	*prev;

	prev = NULL;
	if (!parent || !child || child->parent != parent)
		return (false);
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
		ui_element_destroy(child, mlx, true);
	return (true);
}

void	destroy_ui_element_recursive(t_ui_element *element, t_ui *ui)
{
	t_ui_element	*child;
	t_ui_element	*next;

	if (!element || !ui)
		return ;
	child = element->first_child;
	while (child)
	{
		next = child->next_sibling;
		ui_element_remove_child(element, child, true, ui->mlx);
		child = next;
	}
	if (element->parent)
		ui_element_remove_child(element->parent, element, false, NULL);
	ui_element_destroy(element, ui->mlx, true);
}

t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = (t_ui_element *)ft_calloc(1, sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->visible = true;
	element->style.bg_color = 0x00000000;
	element->style.text_color = 0xFFFFFFFF;
	element->style.border_color = 0x00000000;
	element->style.padding = 2;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->image = NULL;
	element->data = NULL;
	element->action = NULL;
	element->destroy = NULL;
	element->render = NULL;
	element->update = NULL;
	ui_element_setup_handlers(element);
	return (element);
}
