/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_layout.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 11:39:13 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/08 16:56:22 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

struct s_ui_sections g_sections[] = {
	{8.3f, create_camera_section},
	{8.3f, create_light_section},
	{12.0f, create_ambient_section},
	{0.1f, create_dof_section},
	{1.0f, create_sample_section}
};

void	init_value_button_data(t_ui_vbtn *value_btn, const t_vbtn_config *cfg)
{
	if (!value_btn || !cfg)
		return ;
	value_btn->value = cfg->value;
	value_btn->range = cfg->range;
	value_btn->step = cfg->step;
	value_btn->on_click = NULL;
	value_btn->param = NULL;
	value_btn->formatter = cfg->formatter;
}

void	add_inc_dec_buttons(t_ui_element *container,
						const t_vbtn_config *cfg, t_ui_vbtn *value_btn)
{
	t_ui_element	*decr_btn;
	t_ui_element	*incr_btn;

	(void)value_btn;
	decr_btn = create_button(cfg->ctx, "-",
		g_v2f_zero,
		init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		decrement_value_button, cfg->ctx);
	incr_btn = create_button(cfg->ctx, "+",
		init_v2f(cfg->size.x - UI_BUTTON_WIDTH, 0),
		init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		increment_value_button, cfg->ctx);
	if (decr_btn)
		attach_child(container, decr_btn);
	if (incr_btn)
		attach_child(container, incr_btn);
}

void	add_value_label(t_ui_element *container,
	const t_vbtn_config *cfg, t_ui_vbtn *value_btn)
{
	char		*value_str;
	float		label_width;
	float		label_x;
	t_ui_element	*label;

	if (cfg->formatter)
		value_str = ft_strdup(cfg->formatter(*cfg->value));
	else
		value_str = ft_ftoa(*cfg->value, 2);
	if (!value_str)
		return;
	label_width = ft_strlen(value_str) * UI_CHAR_WIDTH;
	label_x = (cfg->size.x - label_width) / 2.0f;
	label = create_label(cfg->ctx, value_str,
		init_v2f(label_x, (cfg->size.y - UI_FONT_HEIGHT) / 2),
		UI_TEXT_COLOR);
	if (label)
	{
		value_btn->value_label = label;
		attach_child(container, label);
	}
	free(value_str);
}

t_ui_element	*create_ui_sections(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size)
{
	const int		num_sections = sizeof(g_sections) / sizeof(g_sections[0]);
	t_ui_element	*panel;
	t_v2f			section_size;
	t_v2f			section_pos;
	int				i;

	panel = create_panel(ctx, pos, size);
	if (!panel)
		return (NULL);
	section_pos = init_v2f(UI_PANEL_PADDING, UI_PANEL_PADDING);
	section_size = init_v2f(size.x - (2 * UI_PANEL_PADDING), 0);
	i = 0;
	while (i < num_sections)
	{
		section_size.y = UI_HEADER_HEIGHT + g_sections[i].height_scale * UI_ROW_HEIGHT;
		attach_child(panel, g_sections[i].create_func(ctx, sample, section_pos, section_size));
		section_pos.y += section_size.y;
		i++;
	}
	return (panel);
}


