/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 22:38:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/13 18:52:26 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char	*ft_ftoa(float f, int precision)
{
	int		int_part;
	float	decimal;
	int		decimal_part;
	char	*str;
	int		len;

	if (precision < 0)
		precision = 0;
	len = 1 + 10 + 1 + precision + 1;
	str = (char *)ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	int_part = (int)f;
	decimal = f - int_part;
	decimal_part = (int)(decimal * pow(10, precision) + 0.5f);
	if (precision > 0)
		snprintf(str, len, "%d.%0*d", int_part, precision, decimal_part);
	else
		snprintf(str, len, "%d", int_part);
	return (str);
}

char	*format_float_value(float value)
{
	static char	buf[16];

	snprintf(buf, sizeof(buf), "%.2f", value);
	return (buf);
}

char	*format_color_value(float value)
{
	static char	buf[16];

	snprintf(buf, sizeof(buf), "%6.2f", value * 255.0f);
	return (buf);
}

t_light	*find_light(t_scene *scene, t_light_type type)
{
	t_light	*light;
	int		i;

	i = 0;
	while (i < (int)scene->lights.size)
	{
		light = (t_light *)vector_get(&scene->lights, i);
		if (light->type == type)
			return (light);
		i++;
	}
	return (NULL);
}

void	ui_mark_dirty(t_ui_context *ctx)
{
	if (ctx)
	{
		ctx->is_dirty = true;
		ctx->needs_redraw = true;
	}
}
