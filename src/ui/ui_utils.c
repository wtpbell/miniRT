/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 22:38:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/18 11:03:58 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_snprintf.h"

char	*format_float_value(float value)
{
	static char	buf[32];

	rt_snprintf(buf, sizeof(buf), "%f", value);
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

uint32_t	blend_colors(uint32_t bg, uint32_t fg)
{
	float	alpha;
	float	inv_alpha;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	alpha = ((fg >> 24) & 0xFF) / 255.0f;
	inv_alpha = 1.0f - alpha;
	r = (uint8_t)(((bg >> 16) & 0xFF) * inv_alpha
			+ ((fg >> 16) & 0xFF) * alpha);
	g = (uint8_t)(((bg >> 8) & 0xFF) * inv_alpha
			+ ((fg >> 8) & 0xFF) * alpha);
	b = (uint8_t)((bg & 0xFF) * inv_alpha + (fg & 0xFF) * alpha);
	return ((0xFFu << 24) | (r << 16) | (g << 8) | b);
}
