/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_display.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 08:59:47 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 16:37:18 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "perlin_display.h"

#include "random.h"
#include "rt_math.h"
#include "minirt.h"

void	print_flt(t_val real, const char *name)
{
	printf("%s: %f\n", name, *real.f);
}

void	print_int(t_val real, const char *name)
{
	printf("%s: %i\n", name, *real.i);
}

void	delta_flt(t_val real, t_val delta)
{
	*real.f += *delta.f;
}

void	delta_int(t_val real, t_val delta)
{
	*real.i += *delta.i;
}

void	double_flt(t_val real, t_val ctx)
{
	(void)ctx;
	*real.f *= 2.0f;
}

void	half_flt(t_val real, t_val ctx)
{
	(void)ctx;
	*real.f *= 0.5f;
}

bool	init_ui(t_pdisplay	*display, t_perlin	*data)
{
	int				i;
	const t_val_mod	m[] = {
		(t_val_mod){"delta-x", {&display->fdelta.x}, {NULL}, double_flt, print_flt},
		(t_val_mod){"delta-x", {&display->fdelta.x}, {NULL}, half_flt, print_flt},
		(t_val_mod){"delta-y", {&display->fdelta.y}, {NULL}, double_flt, print_flt},
		(t_val_mod){"delta-y", {&display->fdelta.y}, {NULL}, half_flt, print_flt},
		(t_val_mod){"uni_scale", {&display->offset.z}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"uni_scale", {&display->offset.z}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"offset-x", {&display->offset.x}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"offset-x", {&display->offset.x}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"offset-y", {&display->offset.y}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"offset-y", {&display->offset.y}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"rate", {&data->rate}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"rate", {&data->rate}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"gain", {&data->gain}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"gain", {&data->gain}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"freq", {&data->freq}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"freq", {&data->freq}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"ampt", {&data->ampt}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"ampt", {&data->ampt}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"layers", {.i = &data->layers}, {.i = &display->idelta.x}, delta_int, print_int},
		(t_val_mod){"layers", {.i = &data->layers}, {.i = &display->idelta.y}, delta_int, print_int},
		(t_val_mod){"dist", {&data->marble.distortion}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"dist", {&data->marble.distortion}, {&display->fdelta.y}, delta_flt, print_flt},
		(t_val_mod){"scale", {&data->marble.scale}, {&display->fdelta.x}, delta_flt, print_flt},
		(t_val_mod){"scale", {&data->marble.scale}, {&display->fdelta.y}, delta_flt, print_flt},
	};
	
	display->size = (sizeof(m) / sizeof(t_val_mod));
	display->values = ft_calloc(display->size, sizeof(t_val_mod));
	if (display->values == NULL)
		return (false);
	i = 0;
	while (i < display->size)
	{
		display->values[i] = m[i];
		++i;
	}
	return (true);
}

void	draw_perlin(mlx_image_t *img, t_perlin *data, t_v3f offset, t_fp_perlin fp_perlin)
{
	t_v2f	point;
	float	val;

	for (uint32_t y = 0; y < img->height; ++y)
	{
		point.y = (y / (float)(img->height - 1) * offset.z) + offset.y;
		for (uint32_t x = 0; x < img->width; ++x)
		{
			point.x = (x / (float)(img->width - 1) * offset.z) + offset.x;
			val = fp_perlin(point, data);
			mlx_put_pixel(img, x, y, v3f_to_col32(init_v3f(val, val, val)));
		}
	}
}

bool	is_key_press(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key && keydata.action == MLX_PRESS);
}

void	call_delta(t_val_mod *fn)
{
	fn->action(fn->value, fn->ctx);
}

void	call_print(t_val_mod *fn)
{
	fn->print(fn->value, fn->name);
}

void	navigate(t_pdisplay *display, mlx_key_data_t keydata)
{
	if (is_key_press(keydata, MLX_KEY_LEFT) && display->curr > 0)
		display->curr -= 2;
	else if (is_key_press(keydata, MLX_KEY_RIGHT) && display->curr < display->size)
		display->curr += 2;
	else
		return ;
	if (display->curr < 0)
		display->curr = 0;
	else if (display->curr >= display->size)
		display->curr = display->size - 2;
	printf("Selected: %i/%i\n", display->curr, display->size);
	call_print(display->values + display->curr);
	call_print(display->values + (display->curr + 1));
}

void	modify(t_pdisplay *display, mlx_key_data_t keydata)
{
	if (is_key_press(keydata, MLX_KEY_UP))
	{
		call_delta(display->values + display->curr);
		call_print(display->values + display->curr);
	}
	else if (is_key_press(keydata, MLX_KEY_DOWN))
	{
		call_delta(display->values + (display->curr + 1));
		call_print(display->values + (display->curr + 1));
	}
}

void	pick_pattern(t_pdisplay *display, mlx_key_data_t keydata)
{
	const t_perlin_node	nodes[4] = {
		{"pink", pink_noise}, {"turb", turbulence_noise}, {"wood", wood_noise},
		{"marble", marble_noise}
	};
	const keys_t		keys[4] = {MLX_KEY_1, MLX_KEY_2, MLX_KEY_3, MLX_KEY_4};
	int					i;

	i = 0;
	while (i < 4)
	{
		if (is_key_press(keydata, keys[i]))
		{
			display->pattern = nodes[i];
			return ;
		}
		++i;
	}
}

void	print_perlin(t_perlin *data)
{
	printf("p_rate:%.3f p_gain:%.3f p_freq:%.3f p_ampt:%.3f\
 p_layers:%i p_dist:%.3f p_scale:%.3f\n",
		data->rate, data->gain, data->freq,
		data->ampt, data->layers,
		data->marble.distortion, data->marble.scale
	);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_pdisplay	*display;

	display = (t_pdisplay *)param;
	if (keydata.action != MLX_PRESS)
		return ;

	if (is_key_press(keydata, MLX_KEY_ESCAPE))
		return (mlx_close_window(display->mlx));
	if (is_key_press(keydata, MLX_KEY_SPACE))
		return (print_perlin(display->p_data));

	navigate(display, keydata);
	pick_pattern(display, keydata);
	modify(display, keydata);
	// display text in window
	draw_perlin(display->img, display->p_data, display->offset,
				display->pattern.fp_perlin);
}

void	perlin_display(void)
{
	mlx_t		*mlx;
	t_pdisplay	display;
	t_perlin	data = (t_perlin)
	{
		2.0f,	// Gain
		0.5f,	// Rate
		1.0f,	// Freq
		1.0f,	// Ampt
		5,		// Layers
		{
			1.0f, //100.0f,	// Distortion
			1.0f //TAU / 200.0f	// Scale
		}
	};

	init_perlin();
	mlx = mlx_init(1600, 900, "miniRT", false);
	display = (t_pdisplay) {
		mlx, &data, NULL, init_v3f(0.0f, 0.0f, 16.0f), 0, -1, NULL, 
		{{1.0f, -1.0f}}, (t_v2i){{1, -1}}, {"pink", pink_noise}
	};
	display.img = mlx_new_image(mlx, 512, 512);
	init_ui(&display, display.p_data);

	draw_perlin(display.img, &data, display.offset, display.pattern.fp_perlin);
	mlx_image_to_window(mlx, display.img, 800 - 256, 450 - 256);
	mlx_key_hook(mlx, key_hook, &display);
	mlx_loop(mlx);
	mlx_terminate(mlx);

	free(display.values);
}
