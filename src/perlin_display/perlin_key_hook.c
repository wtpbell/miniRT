/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_key_hook.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:52:32 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:52:32 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

void	draw_perlin(mlx_image_t *img, t_perlin *data,
		t_v3f offset, t_fp_perlin fp_perlin)
{
	t_v2f		point;
	float		val;
	uint32_t	y;
	uint32_t	x;

	y = 0;
	while (y < img->height)
	{
		point.y = (y / (float)(img->height - 1) * offset.z) + offset.y;
		x = 0;
		while (x < img->width)
		{
			point.x = (x / (float)(img->width - 1) * offset.z) + offset.x;
			val = fp_perlin(point, data);
			mlx_put_pixel(img, x, y, v3f_to_col32(init_v3f(val, val, val)));
			x++;
		}
		y++;
	}
}

static void	navigate(t_pdisplay *d, mlx_key_data_t keydata)
{
	int	prev_i;

	prev_i = d->curr;
	if (is_key_press(keydata, MLX_KEY_DOWN))
		d->curr = (d->curr + 1) % d->param_count;
	else if (is_key_press(keydata, MLX_KEY_UP))
		d->curr = (d->curr - 1 + d->param_count) % d->param_count;
	if (prev_i != d->curr)
	{
		if (prev_i >= 0 && prev_i < d->param_count && d->params[prev_i].row)
			row_style(d->params[prev_i].row, false);
		if (d->curr >= 0 && d->curr < d->param_count && d->params[d->curr].row)
			row_style(d->params[d->curr].row, true);
		ui_mark_dirty(d->ui->context);
	}
}

static void	update_perlin_header(t_pdisplay *d)
{
	t_ui_element	*header;
	t_ui_element	*label;
	char			header_text[128];

	header = d->header;
	label = find_child_by_type(header, UI_LABEL);
	if (!label)
		return ;
	rt_snprintf(header_text, sizeof(header_text),
		"PERLIN NOISE: %s", d->pattern.name);
	free(((t_ui_label *)label->data)->text);
	((t_ui_label *)label->data)->text = ft_strdup(header_text);
	if (!((t_ui_label *)label->data)->text)
		return ;
	ui_mark_dirty(d->ui->context);
}

static void	pick_pattern(t_pdisplay *d, mlx_key_data_t keydata)
{
	const keys_t	keys[4] = {MLX_KEY_1, MLX_KEY_2, MLX_KEY_3, MLX_KEY_4};
	int				i;

	i = 0;
	while (i < 4)
	{
		if (is_key_press(keydata, keys[i]))
		{
			d->pattern = g_nodes[i];
			update_perlin_header(d);
			break ;
		}
		i++;
	}
}

void	perlin_key_hook(mlx_key_data_t keydata, void *param)
{
	t_pdisplay		*display;
	int				prev_curr;

	display = (t_pdisplay *)param;
	if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
		return ;
	if (is_key_press(keydata, MLX_KEY_ESCAPE))
		return (mlx_close_window(display->mlx));
	if (is_key_press(keydata, MLX_KEY_SPACE))
		return (print_perlin(display->p_data));
	prev_curr = display->curr;
	navigate(display, keydata);
	pick_pattern(display, keydata);
	if (keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
		modify(display, keydata);
	if (prev_curr != display->curr
		|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
		|| keydata.key == MLX_KEY_LEFT || keydata.key == MLX_KEY_RIGHT)
		ui_mark_dirty(display->ui->context);
}
