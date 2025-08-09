/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_dof_section.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:28:06 by bewong            #+#    #+#             */
/*   Updated: 2025/08/08 23:19:15 by bewong           ###   ########.fr       */
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
	cfg.step = 0.1f;
	cfg.pos = init_v2f(UI_PADDING, 0);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "DOF", size.x -(UI_PADDING * 2)));
}

t_ui_element	*create_dof_section(t_ui_context *ctx, t_sample *sample,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	t_cam			*camera;

	(void)sample;
	camera = &ctx->scene->camera;
	size.y = UI_HEADER_HEIGHT;
	section = create_panel(ctx, pos, size);
	section->style.bg_color = UI_COLOR_DOF_SECTION;
	if (!section || !camera)
		return (section);
	add_dof_control(ctx, camera, section, size);
	return (section);
}
