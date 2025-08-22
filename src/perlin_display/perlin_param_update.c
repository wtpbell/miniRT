/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_param_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:37:53 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:37:53 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

static void	update_ui_display(t_pdisplay *display, int i)
{
	char		str[32];
	t_val_mod	*mod;

	ft_bzero(str, sizeof(str));
	if (i < 0 || i >= PARAMS_COUNT || !display->params[i].label)
		return ;
	mod = &display->values[i * 2];
	if (mod->action == delta_int && mod->value.i)
		rt_snprintf(str, sizeof(str), "%d", *mod->value.i);
	else if ((mod->action == delta_flt || mod->action == half_flt
			|| mod->action == double_flt) && mod->value.f)
		rt_snprintf(str, sizeof(str), "%f", *mod->value.f);
	else
		return ;
	free(display->params[i].label->text);
	display->params[i].label->text = ft_strdup(str);
	if (!display->params[i].label->text)
		return ;
	if (display->ui && display->ui->context)
		ui_mark_dirty(display->ui->context);
}

void	modify(t_pdisplay *display, mlx_key_data_t keydata)
{
	int			base_idx;
	int			mod_idx;
	t_val_mod	*mod;

	base_idx = display->curr * 2;
	if (base_idx < 0 || base_idx + 1 >= display->size)
		return ;
	if (is_key_press(keydata, MLX_KEY_LEFT))
		mod_idx = base_idx + 1;
	else if (is_key_press(keydata, MLX_KEY_RIGHT))
		mod_idx = base_idx;
	else
		return ;
	mod = &display->values[mod_idx];
	call_delta(mod);
	update_ui_display(display, display->curr);
}
