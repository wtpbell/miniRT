/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 22:38:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/22 15:33:09 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "perlin_display.h"
#include "rt_snprintf.h"

char	*format_float_value(float value)
{
	static char	buf[32];

	rt_snprintf(buf, sizeof(buf), "%f", value);
	return (buf);
}

char	*format_power_of_two(float value)
{
	static char	buf[32];
	float		p;
	float		snapped;

	if (value <= 0.0f)
		value = fmin(1.0f, 128.0f);
	p = roundf(log2f(value));
	snapped = fmaxf(1.0f, fminf(powf(2.0f, p), 128.0f));
	rt_snprintf(buf, sizeof(buf), "%f", snapped);
	return (buf);
}

char	*format_color_value(float value)
{
	static char	buf[32];

	rt_snprintf(buf, sizeof(buf), "%f", value * 255.0f);
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
