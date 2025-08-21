/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:32:10 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:32:10 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

void	print_perlin(t_perlin *data)
{
	printf("p_rate:%.3f p_gain:%.3f p_freq:%.3f p_ampt:%.3f \
p_layers:%i p_dist:%.3f p_scale:%.3f\n",
		data->rate, data->gain, data->freq,
		data->ampt, data->layers,
		data->marble.distortion, data->marble.scale
		);
}

void	cleanup_display(t_pdisplay *display)
{
	if (display->img)
		mlx_delete_image(display->mlx, display->img);
	if (display->ui)
		destroy_ui(display->ui);
	if (display->values)
		free(display->values);
	if (display->mlx)
		mlx_terminate(display->mlx);
}

t_ui_element	*find_child_by_type(t_ui_element *parent, t_ui_type type)
{
	t_ui_element	*child;

	if (!parent)
		return (NULL);
	child = parent->first_child;
	while (child)
	{
		if (child->type == type)
			return (child);
		child = child->next_sibling;
	}
	return (NULL);
}

bool	is_key_press(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key && (keydata.action == MLX_PRESS
			|| keydata.action == MLX_REPEAT));
}

void	call_delta(t_val_mod *mod)
{
	mod->action(mod->value, mod->ctx);
}
