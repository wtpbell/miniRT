/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 22:38:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/12 17:53:57 by bewong        ########   odam.nl         */
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

	if (precision < 0)
		precision = 0;
	int_part = (int)f;
	decimal = f - int_part;
	decimal_part = (int)(decimal * pow(10, precision) + 0.5f);
	str = (char *)calloc(32, sizeof(char));
	if (!str)
		return (NULL);
	if (precision > 0)
		snprintf(str, 2, "%d.%0*d", int_part, precision, decimal_part);
	else
		snprintf(str, 2, "%d", int_part);
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

void	render_button_clicked(t_ui_element *button, void *param)
{
	t_ui_context	*ctx;
	t_game			*game;

	(void)button;
	if (!param)
		return ;
	ctx = (t_ui_context *)param;
	game = (t_game *)ctx->game;
	if (game)
	{
		game->needs_redraw = true;
		update_camera_view(&game->scene->camera);
		printf("Re-render triggered!\n");
	}
}
