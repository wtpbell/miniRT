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

static void	update_int_param(t_pdisplay *d, float step, char *buf, size_t size)
{
	int	*value;

	value = (int *)d->params[d->curr].value;
	if (step > 0)
		(*value)++;
	else
		(*value)--;
	*value = fmax(1, fmin(10, *value));
	rt_snprintf(buf, size, "%d", *value);
}

static void	update_float_param(t_pdisplay *d, float step,
			char *buf, size_t size)
{
	float	*value;
	t_v2f	range;

	value = (float *)d->params[d->curr].value;
	*value += step;
	range = d->params[d->curr].range;
	*value = fmaxf(range.x, fminf(range.y, *value));
	rt_snprintf(buf, size, "%f", *value);
}

void	modify(t_pdisplay *display, mlx_key_data_t keydata)
{
	float	step;
	char	str[32];

	step = 0.0f;
	if (keydata.key == MLX_KEY_RIGHT)
		step = 0.1f;
	else if (keydata.key == MLX_KEY_LEFT)
		step = -0.1f;
	else
		return ;
	if (display->curr == 9)
		update_int_param(display, step, str, sizeof(str));
	else
		update_float_param(display, step, str, sizeof(str));
	if (display->params[display->curr].label)
	{
		free(display->params[display->curr].label->text);
		display->params[display->curr].label->text = ft_strdup(str);
		if (!display->params[display->curr].label->text)
			return ;
		ui_mark_dirty(display->ui->context);
	}
}
