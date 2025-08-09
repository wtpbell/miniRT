/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_sample_section.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 18:24:31 by bewong            #+#    #+#             */
/*   Updated: 2025/08/08 23:27:13 by bewong           ###   ########.fr       */
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
	cfg.step = 2.0f;
	cfg.pos = init_v2f(UI_PADDING, 0);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "SAMPLE", size.x - (UI_PADDING * 2)));
}

static void	add_max_depth_control(t_ui_context *ctx, t_sample *sample,
	t_ui_element *section, t_v2f size)
{
	t_vbtn_config	cfg;
	
	cfg.ctx = ctx;
	cfg.value = &sample->max_depth;
	cfg.range = init_v2f(0.1f, FLT_MAX);
	cfg.step = 1.0f;
	cfg.pos = init_v2f(UI_PADDING, UI_ROW_HEIGHT + UI_PADDING);
	cfg.size = g_v2f_zero;
	cfg.formatter = format_float_value;
	attach_child(section, create_labeled_control(&cfg, "MAX DEPTH", size.x - (UI_PADDING * 2)));
}

t_ui_element	*create_sample_section(t_ui_context *ctx, t_sample *sample,
	t_v2f pos, t_v2f size)
{
	t_ui_element	*section;
	t_v2f			section_size;

	// Always use our calculated size, ignore the passed size.y
	section_size = init_v2f(
		size.x,  // Keep original width
		(UI_ROW_HEIGHT * 2) + (UI_PADDING)  // Header + 2 rows + padding
	);
	
	printf("Creating sample section at (%.1f, %.1f) with size (%.1f, %.1f)\n", 
		pos.x, pos.y, section_size.x, section_size.y);

	section = create_panel(ctx, pos, section_size);
	if (!section || !sample)
		return (section);
		
	section->style.bg_color = UI_COLOR_SAMPLE_SECTION;
	
	// Add controls using the panel's size
	add_sample_control(ctx, sample, section, section_size);
	add_max_depth_control(ctx, sample, section, section_size);
	
	return (section);
}
