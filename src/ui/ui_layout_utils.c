/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layout_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:05:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 16:05:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

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
	t_btn_config	decr_cfg;
	t_btn_config	incr_cfg;

	(void)value_btn;
	decr_cfg = (t_btn_config){
		.ctx = cfg->ctx,
		.label_text = "-",
		.pos = g_v2f_zero,
		.size = init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		.on_click = decrement_value_button,
		.param = (void *)cfg->ctx
	};
	incr_cfg = (t_btn_config){
		.ctx = cfg->ctx,
		.label_text = "+",
		.pos = init_v2f(cfg->size.x - UI_BUTTON_WIDTH, 0),
		.size = init_v2f(UI_BUTTON_WIDTH, cfg->size.y),
		.on_click = increment_value_button,
		.param = (void *)cfg->ctx
	};
	if (create_button(&decr_cfg))
		attach_child(container, create_button(&decr_cfg));
	if (create_button(&incr_cfg))
		attach_child(container, create_button(&incr_cfg));
}
