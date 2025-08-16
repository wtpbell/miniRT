/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:05:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 19:05:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	init_ui_context(t_ui_context *ctx, mlx_t *mlx,
						t_scene *scene, void *game_ptr)
{
	if (!ctx)
		return ;
	ctx->mlx = mlx;
	ctx->scene = scene;
	ctx->game = game_ptr;
	ctx->is_visible = true;
	ctx->needs_redraw = true;
}

static t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene,
		void *game_ptr)
{
	t_ui_context	*ctx;
	int				panel_width;

	ctx = (t_ui_context *)ft_calloc(1, sizeof(t_ui_context));
	if (!ctx)
		return (NULL);
	init_ui_context(ctx, mlx, scene, game_ptr);
	panel_width = UI_PANEL_WIDTH;
	if (panel_width > mlx->width / 2)
		panel_width = mlx->width / 2;
	ctx->canvas = mlx_new_image(mlx, panel_width, mlx->height);
	if (!ctx->canvas)
		return (destroy_ui_context(ctx), NULL);
	if (panel_width > (int)mlx->width)
		panel_width = mlx->width;
	if (mlx_image_to_window(mlx, ctx->canvas, 0, 0) < 0)
		return (destroy_ui_context(ctx), NULL);
	return (ctx);
}

static t_ui_element	*create_ui_section(t_ui *ui, t_sample *sample, t_v2f size)
{
	t_section_config	cfg;

	cfg = (t_section_config){
		.ctx = ui->context,
		.sample = sample,
		.pos = g_v2f_zero,
		.size = size
	};
	return (create_ui_sections(&cfg));
}

t_ui	*create_ui(mlx_t *mlx, t_scene *scene, t_sample *sample, void *game_ptr)
{
	t_ui			*ui;
	t_ui_element	*ui_sections;
	t_v2f			size;

	if (!mlx || !game_ptr)
		return (NULL);
	ui = (t_ui *)ft_calloc(1, sizeof(t_ui));
	if (!ui)
		return (NULL);
	ui->context = create_ui_context(mlx, scene, game_ptr);
	if (!ui->context)
		return (free(ui), NULL);
	size = init_v2f(UI_PANEL_WIDTH, HEIGHT);
	ui->root = create_panel(ui->context, g_v2f_zero, size);
	if (!ui->root)
		return (destroy_ui(ui), NULL);

	// Only create UI sections if we have a scene
	if (scene != NULL)
	{
		ui_sections = create_ui_section(ui, sample, size);
		if (!ui_sections)
			return (destroy_ui(ui), NULL);
		attach_child(ui->root, ui_sections);
	}

	return (ui);
}
