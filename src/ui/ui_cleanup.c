/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:42:36 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 11:01:43 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "libft.h"
#include <stdlib.h>

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
		destroy_ui_element_recursive(child, ctx);
		child = next;
	}
	element->first_child = NULL;
	element->parent = NULL;
	destroy_ui_element(element, ctx);
}

static void	ui_images_destroy(mlx_t *mlx, t_ui_images *images)
{
	if (!images || !mlx)
		return ;

	if (images->button_img)
		mlx_delete_image(mlx, images->button_img);
	if (images->header_img)
		mlx_delete_image(mlx, images->header_img);
	if (images->panel_img)
		mlx_delete_image(mlx, images->panel_img);
	
	// Clear the pointers
	images->button_img = NULL;
	images->header_img = NULL;
	images->panel_img = NULL;
}

void	destroy_ui_context(t_ui_context *ctx)
{
	if (!ctx)
		return ;

	// Delete canvas if it exists
	if (ctx->canvas)
	{
		if (ctx->mlx)
			mlx_delete_image(ctx->mlx, ctx->canvas);
		ctx->canvas = NULL;
	}

	// Delete images if they exist
	if (ctx->images)
	{
		ui_images_destroy(ctx->mlx, ctx->images);
		free(ctx->images);  
		ctx->images = NULL;
	}

	// Clear all pointers
	ctx->mlx = NULL;
	ctx->scene = NULL;
	ctx->game = NULL;

	free(ctx);
}

void	destroy_ui(t_ui *ui)
{
	if (!ui)
		return ;
	
	printf("Starting UI destruction\n");
	if (ui->root)
	{
		printf("Destroying root element\n");
		destroy_ui_element_recursive(ui->root, ui->context);
		ui->root = NULL;
	}
	if (ui->context)
	{
		printf("Destroying UI context\n");
		destroy_ui_context(ui->context);
		ui->context = NULL;
	}
	printf("Freeing UI structure\n");
	free(ui);
}
