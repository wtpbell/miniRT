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
	float			*values[] = {&camera->t.pos.x, &camera->t.pos.y, &camera->t.pos.z};
	int				i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = values[i];
		cfg.range = init_v2f(-1000.0f, 1000.0f);
		cfg.step = 0.1f;
		cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + UI_PADDING + i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(&cfg, labels[i], size.x - (UI_PADDING * 2)));
		i++;
	}
}

static void	add_camera_dir_controls(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	const char		*labels[] = {"DIR X", "DIR Y", "DIR Z"};
	float			*values[3];
	int				i;

	values[0] = &camera->t.dir.x;
	values[1] = &camera->t.dir.y;
	values[2] = &camera->t.dir.z;
	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = values[i];
		cfg.range = init_v2f(-1.0f, 1.0f);
		cfg.step = 0.01f;
		cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + UI_PADDING + 3 * (UI_ROW_HEIGHT + UI_PADDING) + 
			i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_float_value;
		attach_child(section, create_labeled_control(&cfg, labels[i], size.x - (UI_PADDING * 2)));
		i++;
	}
}

static void	add_camera_fov_control(t_ui_context *ctx, t_cam *camera,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &camera->fov;
	cfg.range = init_v2f(0.0f, 180.0f);
	cfg.step = 1.0f;
	cfg.pos = init_v2f(UI_PADDING,
		UI_HEADER_HEIGHT + UI_PADDING + 6 * (UI_ROW_HEIGHT + UI_PADDING));
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "FOV", size.x - (UI_PADDING * 2)));
}

t_ui_element	*create_camera_section(t_ui_context *ctx, t_scene *scene,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	t_cam			*camera;

	if (!scene)
		return (NULL);
	camera = &scene->camera;
	size.y = UI_HEADER_HEIGHT + 7 * (UI_ROW_HEIGHT + UI_PADDING);
	section = create_panel(ctx, pos, size);
	if (!section || !camera)
		return (section);
	attach_child(section, create_header(ctx, "CAMERA",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT)));
	add_camera_pos_controls(ctx, camera, section, size);
	add_camera_dir_controls(ctx, camera, section, size);
	add_camera_fov_control(ctx, camera, section, size);
	return (section);
}
