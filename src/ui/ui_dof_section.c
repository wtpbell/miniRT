/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_dof_section.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:28:06 by bewong            #+#    #+#             */
/*   Updated: 2025/08/09 18:39:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	add_dof_control(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &camera->focus_dist;
	cfg.range = init_v2f(0.1f, FLT_MAX);
	cfg.step = 0.5f;
	cfg.pos = init_v2f(UI_PADDING, 0);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(
			&cfg, "DOF", size.x - (2 * UI_PADDING)));
}

t_ui_element	*create_dof_section(t_section_config *cfg)
{
	t_ui_element	*section;
	t_cam			*camera;
	t_v2f			size;

	if (!cfg || !cfg->ctx || !cfg->ctx->scene)
		return (NULL);
	camera = &cfg->ctx->scene->camera;
	size = init_v2f(cfg->size.x, UI_HEADER_HEIGHT);
	section = create_panel(cfg->ctx, cfg->pos, size);
	if (!section || !camera)
		return (section);
	section->style.bg_color = UI_COLOR_DOF_SECTION;
	add_dof_control(cfg->ctx, camera, section, size);
	return (section);
}
