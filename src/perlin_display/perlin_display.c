/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 08:59:47 by jboon             #+#    #+#             */
/*   Updated: 2025/08/17 13:26:42 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "perlin_display.h"

#include "random.h"
#include "rt_math.h"
#include "minirt.h"
#include "game.h"
#include "ui.h"
#include "rt_snprintf.h"

struct s_params g_params[10]; 

void	print_flt(t_val real, const char *name)
{
	printf("%s: %.12f\n", name, *real.f);
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

static void	*ft_memdup(const void *src, size_t n)
{
	void	*dest;

	dest = malloc(n);
	if (!dest)
		return (NULL);
	return (ft_memcpy(dest, src, n));
}

bool	init_ui(t_pdisplay	*display, t_perlin	*data)
{
	const t_val_mod m[] = {
		{"delta-x", {.f = &display->fdelta.x}, {.f = NULL}, double_flt},
		{"delta-x", {.f = &display->fdelta.x}, {.f = NULL}, half_flt},
		{"uni_scale", {.f = &display->offset.z}, {.f = NULL}, double_flt},
		{"uni_scale", {.f = &display->offset.z}, {.f = NULL}, half_flt},
		{"offset-x", {.f = &display->offset.x}, {.f = NULL}, double_flt},
		{"offset-x", {.f = &display->offset.x}, {.f = NULL}, half_flt},
		{"offset-y", {.f = &display->offset.y}, {.f = NULL}, double_flt},
		{"offset-y", {.f = &display->offset.y}, {.f = NULL}, half_flt},
		{"rate", {.f = &data->rate}, {.f = NULL}, double_flt},
		{"rate", {.f = &data->rate}, {.f = NULL}, half_flt},
		{"gain", {.f = &data->gain}, {.f = NULL}, double_flt},
		{"gain", {.f = &data->gain}, {.f = NULL}, half_flt},
		{"freq", {.f = &data->freq}, {.f = NULL}, double_flt},
		{"freq", {.f = &data->freq}, {.f = NULL}, half_flt},
		{"ampt", {.f = &data->ampt}, {.f = NULL}, double_flt},
		{"ampt", {.f = &data->ampt}, {.f = NULL}, half_flt},
		{"layers", {.i = &data->layers}, {.i = &display->idelta.x}, delta_int},
		{"layers", {.i = &data->layers}, {.i = &display->idelta.y}, delta_int},
		{"dist", {.f = &data->marble.distortion}, {.f = NULL}, double_flt},
		{"dist", {.f = &data->marble.distortion}, {.f = NULL}, half_flt},
		{"scale", {.f = &data->marble.scale}, {.f = NULL}, double_flt},
		{"scale", {.f = &data->marble.scale}, {.f = NULL}, half_flt}
	};
	display->size = sizeof(m) / sizeof(t_val_mod);
	display->values = ft_memdup(m, sizeof(m));
	return (display->values != NULL);
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

t_ui_element *find_child_by_type(t_ui_element *parent, t_ui_type type)
{
	t_ui_element *child;

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

void navigate(t_pdisplay *display, mlx_key_data_t keydata)
{
	int	max_params;

	max_params = display->param_count - 1;
	if (keydata.key == MLX_KEY_RIGHT)
		display->curr = (display->curr + 1) % display->param_count;
	else if (keydata.key == MLX_KEY_LEFT)
		display->curr = (display->curr - 1 + display->param_count) % display->param_count;
	printf("Current parameter: %d / %d\n", display->curr, max_params);
}

void	modify(t_pdisplay *display, mlx_key_data_t keydata)
{
	float	step;
	float	*value_f;
	int		*value_i;
	t_v2f	range;
	char	str[32];

	if (display->curr < 0 || display->curr >= display->param_count)
		return ;
	printf("Modifying parameter %d\n", display->curr);
	if (keydata.key == MLX_KEY_UP)
		step = 0.1f;
	else if (keydata.key == MLX_KEY_DOWN)
		step = -0.1f;
	if (!(keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN))
		return ;
	if (display->curr == 9)
	{
		value_i = (int *)display->params[display->curr].value;
		if (step > 0)
			*value_i += 1;
		else
			*value_i -= 1;
		*value_i = fmax(1, fmin(10, *value_i));
		rt_snprintf(str, sizeof(str), "%d", *value_i);
	}
	else
	{
		value_f = (float *)display->params[display->curr].value;
		*value_f += step;
		range = display->params[display->curr].range;
		*value_f = fmaxf(range.x, fminf(range.y, *value_f));
		rt_snprintf(str, sizeof(str), "%f", *value_f);
	}
	if (display->params[display->curr].label)
	{
		if (display->params[display->curr].label->text)
			free(display->params[display->curr].label->text);
		display->params[display->curr].label->text = ft_strdup(str);
		ui_mark_dirty(display->ui->context);
	}
	draw_perlin(display->img, display->p_data, display->offset,
			   display->pattern.fp_perlin);
}

void pick_pattern(t_pdisplay *display, mlx_key_data_t keydata)
{
	const t_perlin_node	nodes[4] = {
		{"PINK", pink_noise},
		{"TURB", turbulence_noise},
		{"WOOD", wood_noise},
		{"MARBLE", marble_noise}
	};
	const keys_t		keys[4] = {MLX_KEY_1, MLX_KEY_2, MLX_KEY_3, MLX_KEY_4};
	char				header_text[128];
	t_ui_element		*header;
	t_ui_element		*label;
	int					i;

	i = 0;
	while (i < 4)
	{
		if (is_key_press(keydata, keys[i]))
		{
			display->pattern = nodes[i];
			draw_perlin(display->img, display->p_data, display->offset, nodes[i].fp_perlin);
			
			if (display->ui && display->ui_panel && display->header)
			{
				header = display->header;
				label = find_child_by_type(header, UI_LABEL);
				
				if (label)
				{
					rt_snprintf(header_text, sizeof(header_text),
								"PERLIN NOISE: %s", nodes[i].name);
					if (((t_ui_label *)label->data)->text)
						free(((t_ui_label *)label->data)->text);
					((t_ui_label *)label->data)->text = ft_strdup(header_text);
					ui_mark_dirty(display->ui->context);
				}
			}
			break ;
		}
		i++;
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

void set_pattern(t_ui_element *btn, void *param)
{
	t_perlin_node	*node;
	t_pdisplay		*display;
	t_ui_element	*panel;
	t_ui_element	*header;
	t_ui_element	*label;
	char			header_text[128];

	(void)param;
	node = ((t_ui_btn *)btn->data)->param;
	display = ((t_ui_btn *)btn->parent->parent->data)->param;
	display->pattern = *node;
	draw_perlin(display->img, display->p_data, display->offset, node->fp_perlin);
	if (display->ui && display->ui_panel)
	{
		panel = display->ui_panel;
		header = find_child_by_type(panel, UI_HEADER);
		if (header)
		{
			label = find_child_by_type(header, UI_LABEL);
			if (label)
			{
				rt_snprintf(header_text, sizeof(header_text),
						  "PERLIN NOISE: %s", node->name);
				if (((t_ui_label *)label->data)->text)
					free(((t_ui_label *)label->data)->text);
				((t_ui_label *)label->data)->text = ft_strdup(header_text);
				ui_mark_dirty(display->ui->context);
			}
		}
	}
}

static void	init_params(t_pdisplay *display)
{
	const struct s_params	params[PARAMS_COUNT] = {
	{"UNI SCALE", &display->offset.z, init_v2f(0.1f, 100.0f)},
	{"OFFSET X", &display->offset.x, init_v2f(-100.0f, 100.0f)},
	{"OFFSET Y", &display->offset.y, init_v2f(-100.0f, 100.0f)},
	{"RATE", &display->p_data->rate, init_v2f(0.1f, 10.0f)},
	{"GAIN", &display->p_data->gain, init_v2f(0.1f, 10.0f)},
	{"FREQ", &display->p_data->freq, init_v2f(0.1f, 10.0f)},
	{"AMPT", &display->p_data->ampt, init_v2f(0.1f, 10.0f)},
	{"DISTORTION", &display->p_data->marble.distortion, init_v2f(0.1f, 10.0f)},
	{"SCALE", &display->p_data->marble.scale, init_v2f(0.1f, 10.0f)},
	{"LAYERS", (float *)&display->p_data->layers, init_v2f(1, 10)}
	};
	ft_memcpy(g_params, params, sizeof(params));
}

void add_parameter_controls(t_ui *ui, t_ui_element *parent, t_pdisplay *display)
{
	t_ui_element	*control;
	t_v2f			label_pos;
	t_v2f			value_pos;
	char			value_str[32];
	int				i;

	display->param_count = sizeof(g_params) / sizeof(g_params[0]);
	i = 0;
	while (i < display->param_count)
	{
		label_pos = init_v2f(UI_LABEL_PADDING * 4,
							UI_HEADER_HEIGHT + UI_LABEL_PADDING + (i * UI_ROW_HEIGHT));
		value_pos = init_v2f(UI_LABEL_WIDTH + (8 * UI_LABEL_PADDING),
							UI_HEADER_HEIGHT + UI_LABEL_PADDING + (i * UI_ROW_HEIGHT));
		control = create_label(ui->context, g_params[i].name, label_pos, UI_TEXT_COLOR);
		attach_child(parent, control);
		if (i == display->param_count - 1)
			rt_snprintf(value_str, sizeof(value_str), "%d", *(int *)g_params[i].value);
		else
			rt_snprintf(value_str, sizeof(value_str), "%f", *g_params[i].value);
		control = create_label(ui->context, value_str, value_pos, UI_TEXT_COLOR);
		attach_child(parent, control);
		display->params[i].value = g_params[i].value;
		display->params[i].range = g_params[i].range;
		display->params[i].label = (t_ui_label *)control->data;
		++i;
	}
}

void setup_perlin_ui(t_ui *ui, t_pdisplay *display)
{
	t_ui_element		*panel;
	t_ui_element		*section;
	t_v2f				pos;
	t_v2f				size;
	char				header_text[128];
	const t_perlin_node	nodes[4] = {
		{"PINK", pink_noise}, 
		{"TURB", turbulence_noise},
		{"WOOD", wood_noise},
		{"MARBLE", marble_noise}
	};

	pos = g_v2f_zero;
	size = init_v2f(UI_PANEL_WIDTH, UI_ROW_HEIGHT * (PARAMS_COUNT + 1));
	display->pattern = nodes[0];
	draw_perlin(display->img, display->p_data, display->offset, nodes[0].fp_perlin);
	panel = create_panel(ui->context, pos, size);
	attach_child(ui->root, panel);
	rt_snprintf(header_text, sizeof(header_text), "PERLIN NOISE: %s", nodes[0].name);
	section = create_header(ui->context, header_text,
		g_v2f_zero, init_v2f(size.x, UI_HEADER_HEIGHT));
	attach_child(panel, section);
	display->header = section;
	display->ui_panel = panel;
	add_parameter_controls(ui, panel, display);
	ui_mark_dirty(ui->context);
}

static void perlin_key_hook(mlx_key_data_t keydata, void *param)
{
	t_pdisplay	*display;

	display = (t_pdisplay *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (is_key_press(keydata, MLX_KEY_ESCAPE))
		return (mlx_close_window(display->mlx));
	navigate(display, keydata);
	pick_pattern(display, keydata);
	modify(display, keydata);
	draw_perlin(display->img, display->p_data, display->offset,
				display->pattern.fp_perlin);
}

static void perlin_render_loop(void *param)
{
	t_ui *ui = (t_ui *)param;

	ft_memset(ui->context->canvas->pixels, 0,
			 ui->context->canvas->width * ui->context->canvas->height * sizeof(uint32_t));
	render_ui_element(ui->root, ui->context);
	ui->context->needs_redraw = false;
}

static void  init_display(mlx_t *mlx, t_pdisplay *display, t_perlin *data)
{
	display->mlx = mlx;
	display->p_data = data;
	display->img = mlx_new_image(mlx, 512, 512);
	display->offset = init_v3f(0.0f, 0.0f, 16.0f);
	display->curr = 0;
	display->size = -1;
	display->values = NULL;
	display->fdelta = init_v2f(1.0f, -1.0f);
	display->idelta = (t_v2i){{1, -1}};
	display->pattern = (t_perlin_node){"pink", pink_noise};
	display->ui = NULL;
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
			1.0f, // Distortion
			1.0f // Scale
		}
	};

	init_perlin();
	mlx = mlx_init(WIDTH, HEIGHT, "Perlin Noise Display", false);
	if (!mlx)
		return ;
	init_display(mlx, &display, &data);
	init_params(&display);
	if (!display.img)
	{
		perror("perlin_display:display.img");
		mlx_terminate(mlx);
		return ;
	}
	display.ui = create_ui(mlx, NULL, NULL, &display);
	if (!display.ui)
		perror("perlin_display");
	else
	setup_perlin_ui(display.ui, &display);
	draw_perlin(display.img, &data, display.offset, display.pattern.fp_perlin);
	mlx_image_to_window(mlx, display.img,
		(WIDTH - display.img->width) / 2,
		(HEIGHT - display.img->height) / 2);
	mlx_key_hook(mlx, perlin_key_hook, &display);
	if (display.ui)
		mlx_loop_hook(mlx, perlin_render_loop, display.ui);
	mlx_loop(mlx);
	if (display.ui)
		destroy_ui(display.ui);
	if (display.values)
		free(display.values);
	mlx_terminate(mlx);
}
