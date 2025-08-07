/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_sample_section.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 18:24:31 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/07 19:19:57 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static void	add_sample_control(t_ui_context *ctx, t_sample *sample,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	
	cfg.ctx = ctx;
	cfg.value = &sample->sample_pxl;
	cfg.range = init_v2f(0.1f, FLT_MAX);
	cfg.step = 0.1f;
	cfg.pos = init_v2f(UI_PADDING, 0);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "DOF", size.x -(UI_PADDING * 2)));
}

t_ui_element	*create_sample_section(t_ui_context *ctx, t_sample *sample,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;

	sample = &sample;
	size.y = UI_HEADER_HEIGHT;
	section = create_panel(ctx, pos, size);
	if (!section || !sample)
		return (section);
	add_sample_control(ctx, sample, section, size);
	return (section);
}
