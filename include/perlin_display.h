/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_display.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 11:39:26 by jboon             #+#    #+#             */
/*   Updated: 2025/08/17 14:21:47 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_DISPLAY_H
# define PERLIN_DISPLAY_H

# include "MLX42/MLX42.h"
# include "rt_math.h"
# include "material.h"
# include "ui.h"

#define PARAMS_COUNT 12

typedef struct s_param
{
	float			*value;
	t_v2f			range;
	t_ui_label		*label;
	t_ui_element	*row;
}t_param;

typedef union u_val
{
	float	*f;
	int		*i;
}	t_val;

typedef struct s_value_modifier
{
	const char	*name;
	t_val		value;
	t_val		ctx;
	void		(*action)(t_val value, t_val ctx);
}	t_val_mod;

typedef struct s_perlin_node
{
	const char	*name;
	t_fp_perlin	fp_perlin;
}	t_perlin_node;

typedef struct s_perlin_display
{
	mlx_t			*mlx;
	t_perlin		*p_data;
	mlx_image_t		*img;
	t_v3f			offset;
	int				curr;
	int				size;
	t_val_mod		*values;
	t_v2f			fdelta;
	t_v2i			idelta;
	t_perlin_node	pattern;
	t_ui			*ui;
	t_ui_element	*ui_panel;
	t_ui_element	*header;
	t_param			params[PARAMS_COUNT];
	int				param_count;
}	t_pdisplay;

struct s_params
{
	const char	*name;
	float		*value;
	t_v2f		range;
};

extern struct s_params g_params[];

void	perlin_display(void);
void	print_perlin(t_perlin *data);
void	pick_pattern(t_pdisplay *display, mlx_key_data_t keydata);
void	modify(t_pdisplay *display, mlx_key_data_t keydata);
void	navigate(t_pdisplay *display, mlx_key_data_t keydata);
void	call_print(t_val_mod *fn);
void	call_delta(t_val_mod *fn);
void	draw_perlin(mlx_image_t *img, t_perlin *data, t_v3f offset, t_fp_perlin fp_perlin);
bool	init_ui(t_pdisplay	*display, t_perlin	*data);
void	half_flt(t_val real, t_val ctx);
void	double_flt(t_val real, t_val ctx);
void	delta_int(t_val real, t_val delta);
void	delta_flt(t_val real, t_val delta);
void	print_int(t_val real, const char *name);
void	print_flt(t_val real, const char *name);
bool	is_key_press(mlx_key_data_t keydata, keys_t key);
#endif
