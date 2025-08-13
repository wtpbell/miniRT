/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_cleanup.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/12 17:42:36 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/13 20:20:21 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	destroy_ui_element_recursive(t_ui_element *element,
				t_ui_context *ctx)
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
	images->button_img = NULL;
	images->header_img = NULL;
	images->panel_img = NULL;
}

void	destroy_ui_context(t_ui_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->canvas)
	{
		if (ctx->mlx)
			mlx_delete_image(ctx->mlx, ctx->canvas);
		ctx->canvas = NULL;
	}
	if (ctx->images)
	{
		ui_images_destroy(ctx->mlx, ctx->images);
		free(ctx->images);
		ctx->images = NULL;
	}
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
