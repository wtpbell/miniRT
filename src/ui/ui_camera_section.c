/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_camera_section.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:25:52 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 18:25:52 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	add_camera_pos_controls(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"POS X", "POS Y", "POS Z"};
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = &camera->t.pos._axis[i];
		cfg.range = init_v2f(MIN_POS, MAX_POS);
		cfg.step = 0.1f;
		cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + i
				* (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(
				&cfg, labels[i], size.x - (2 * UI_PADDING)));
		i++;
	}
}

static void	add_camera_dir_controls(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"DIR X", "DIR Y", "DIR Z"};
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = &camera->t.dir._axis[i];
		cfg.range = init_v2f(MIN_DIR, MAX_DIR);
		cfg.step = 0.1f;
		cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + 3
				* (UI_ROW_HEIGHT + UI_PADDING) + i
				* (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(
				&cfg, labels[i], size.x - (2 * UI_PADDING)));
		i++;
	}
}

static void	add_camera_fov_control(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &camera->fov;
	cfg.range = init_v2f(MIN_FOV, MAX_FOV);
	cfg.step = 1.0f;
	cfg.pos = init_v2f(UI_PADDING, UI_HEADER_HEIGHT + UI_PADDING + 6
			* (UI_ROW_HEIGHT + UI_PADDING));
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(
			&cfg, "FOV", size.x - (2 * UI_PADDING)));
}

t_ui_element	*create_camera_section(t_section_config *cfg)
{
	t_ui_element	*section;
	t_cam			*camera;
	t_v2f			size;

	camera = &cfg->ctx->scene->camera;
	size = init_v2f(cfg->size.x,
			UI_HEADER_HEIGHT + 7 * (UI_ROW_HEIGHT + UI_PADDING) + UI_PADDING);
	section = create_panel(cfg->ctx, cfg->pos, size);
	if (!section)
		return (NULL);
	section->style.bg_color = UI_COLOR_CAMERA_SECTION;
	attach_child(section, create_header(cfg->ctx, "CAMERA",
			g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT)));
	add_camera_pos_controls(cfg->ctx, camera, section, size);
	add_camera_dir_controls(cfg->ctx, camera, section, size);
	add_camera_fov_control(cfg->ctx, camera, section, size);
	return (section);
}
