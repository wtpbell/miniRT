/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:42:36 by bewong            #+#    #+#             */
/*   Updated: 2025/08/14 23:25:26 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	destroy_ui_element_recursive(t_ui_element *element)
{
	t_ui_element	*child;
	t_ui_element	*next;

	child = element->first_child;
	while (child)
	{
		next = child->next_sibling;
		destroy_ui_element_recursive(child);
		child = next;
	}
	element->first_child = NULL;
	element->parent = NULL;
	destroy_ui_element(element);
}

void	destroy_ui_context(t_ui_context *ctx)
{
	if (ctx->canvas)
	{
		if (ctx->mlx)
			mlx_delete_image(ctx->mlx, ctx->canvas);
		ctx->canvas = NULL;
	}
	ctx->mlx = NULL;
	ctx->scene = NULL;
	ctx->game = NULL;
	free(ctx);
}

void	destroy_ui(t_ui *ui)
{
	if (ui->root)
	{
		destroy_ui_element_recursive(ui->root);
		ui->root = NULL;
	}
	if (ui->context)
	{
		destroy_ui_context(ui->context);
		ui->context = NULL;
	}
	free(ui);
}
