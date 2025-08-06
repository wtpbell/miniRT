/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_ambient_section.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:59:58 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 17:59:58 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static t_light *find_ambient_light(t_scene *scene)
{
	t_light	*light;
	int		i;
	
	i = 0;
	while (i < (int)scene->lights.size)
	{
		light = (t_light *)vector_get(&scene->lights, i);
		if (light && light->type == LIGHT_AMBIENT)
			return light;
		i++;
	}
	return (NULL);
}

static void	add_ambient_color_controls(t_ui_context *ctx, t_light *ambient,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	int						i;

	i = 0;
	while (i < 3)
	{
		cfg.ctx = ctx;
		cfg.value = (&ambient->color.x) + i;
		cfg.range = init_v2f(0, 1.0f);
		cfg.step = 1.0f / 255.0f;
		cfg.pos = init_v2f(UI_PADDING,
			UI_HEADER_HEIGHT + UI_PADDING + i * (UI_ROW_HEIGHT + UI_PADDING));
		cfg.size = g_v2f_zero;
		cfg.formatter = format_color_value;
		if (i == 0)
			attach_child(section, create_labeled_control(&cfg, "COLOR R", size.x - (UI_PADDING * 2)));
		else if (i == 1)
			attach_child(section, create_labeled_control(&cfg, "COLOR G", size.x - (UI_PADDING * 2)));
		else
			attach_child(section, create_labeled_control(&cfg, "COLOR B", size.x - (UI_PADDING * 2)));
		i++;
	}
}
static void	add_ambient_intensity_control(t_ui_context *ctx, t_light *ambient,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;

	cfg.ctx = ctx;
	cfg.value = &ambient->intensity;
	cfg.range = init_v2f(0, 1);
	cfg.step = 0.05f;
	cfg.pos = init_v2f(UI_PADDING,
		UI_HEADER_HEIGHT + UI_PADDING + 3 * (UI_ROW_HEIGHT + UI_PADDING));
	cfg.size = g_v2f_zero;
	cfg.formatter = NULL;
	attach_child(section, create_labeled_control(&cfg,
		"INTENSITY", size.x - (UI_PADDING * 2)));
}

t_ui_element	*create_ambient_section(t_ui_context *ctx, t_scene *scene,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	t_light			*ambient;

	ambient = find_ambient_light(scene);
	section = create_panel(ctx, pos, size);
	if (!section || !ambient)
		return (section);
	attach_child(section, create_header(ctx, "AMBIENT LIGHT",
		init_v2f(0, 0), init_v2f(size.x, UI_HEADER_HEIGHT)));
	add_ambient_color_controls(ctx, ambient, section, size);
	add_ambient_intensity_control(ctx, ambient, section, size);
	return (section);
}
