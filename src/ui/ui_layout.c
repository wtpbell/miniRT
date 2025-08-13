/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:39:13 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 16:08:05 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

// Forward declarations
void				render_button_clicked(t_ui_element *button, void *param);
t_ui_element		*create_section(t_ui_context *ctx, const char *title,
						t_v2f pos, t_v2f size);
static t_ui_element	*create_render_button_section(t_section_config *cfg);

struct s_ui_sections	g_sections[] = {
{9.2f, create_camera_section},
{9.2f, create_light_section},
{5.8f, create_ambient_section},
{1.0f, create_dof_section},
{2.0f, create_sample_section},
{0.5f, create_render_button_section}
};

static t_ui_element	*create_render_button_section(t_section_config *cfg)
{
	t_ui_element	*panel;
	t_ui_element	*render_btn;
	t_btn_config	btn_cfg;
	t_v2f			btn_size;

	panel = create_panel(cfg->ctx, cfg->pos,
			init_v2f(cfg->size.x, UI_ROW_HEIGHT * 2.0f));
	if (!panel)
		return (NULL);
	btn_size = init_v2f(cfg->size.x - (2 * UI_PADDING), UI_ROW_HEIGHT * 1.5f);
	btn_cfg = (t_btn_config){
		.ctx = cfg->ctx,
		.label_text = "RENDER",
		.size = btn_size,
		.pos = init_v2f(UI_PADDING, (panel->size.y - btn_size.y) / 2),
		.on_click = render_button_clicked,
		.param = cfg->ctx
	};
	render_btn = create_button(&btn_cfg);
	if (render_btn)
	{
		render_btn->style.bg_color = UI_RENDER_BUTTON_COLOR;
		attach_child(panel, render_btn);
	}
	return (panel);
}

static float	total_height_scale(void)
{
	int		i;
	float	total;
	int		count;

	i = 0;
	total = 0;
	count = sizeof(g_sections) / sizeof(g_sections[0]);
	while (i < count)
	{
		total += g_sections[i].height_scale;
		i++;
	}
	return (total);
}

static void	create_and_attach_sections(t_ui_element *panel,
			t_section_config *cfg, t_v2f pos, t_v2f size)
{
	t_v2f				section_size;
	t_section_config	scfg;
	t_ui_element		*sec;
	int					i;
	int					count;

	i = 0;
	count = sizeof(g_sections) / sizeof(g_sections[0]);
	while (i < count)
	{
		section_size = init_v2f(size.x, g_sections[i].height_scale * size.y);
		scfg = (t_section_config){cfg->ctx, cfg->sample, pos, section_size};
		sec = g_sections[i].create_func(&scfg);
		if (sec)
		{
			attach_child(panel, sec);
			pos.y += sec->size.y + UI_PADDING;
		}
		i++;
	}
}

t_ui_element	*create_ui_sections(t_section_config *cfg)
{
	float			scale_factor;
	t_v2f			pos;
	t_v2f			size;
	t_ui_element	*panel;
	int				count;

	if (!cfg || !cfg->ctx)
		return (NULL);
	count = sizeof(g_sections) / sizeof(g_sections[0]);
	scale_factor = (cfg->size.y - ((count + 1) * UI_PADDING))
		/ total_height_scale();
	size = init_v2f(cfg->size.x - (2 * UI_PADDING), scale_factor);
	panel = create_panel(cfg->ctx, cfg->pos, cfg->size);
	if (!panel)
		return (NULL);
	pos = init_v2f(UI_PADDING, UI_PADDING);
	create_and_attach_sections(panel, cfg, pos, size);
	return (panel);
}
