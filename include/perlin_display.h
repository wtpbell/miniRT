/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_display.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 11:39:26 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/21 14:48:57 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_DISPLAY_H
# define PERLIN_DISPLAY_H

# include "MLX42/MLX42.h"
# include "rt_math.h"
# include "material.h"
# include "ui.h"
# include "rt_snprintf.h"
# include "vector.h"

# define PARAMS_COUNT 12
# define P_POW 32.0f
# define P_IPOW 0.015625f

typedef union u_val
{
	float	*f;
	int		*i;
}	t_val;

typedef struct s_param
{
	const char		*name;
	t_val			value;
	t_v2f			range;
	t_ui_label		*label;
	t_ui_element	*row;
}	t_param;

typedef struct s_value_modifier
{
	const char	*name;
	t_val		value;
	t_val		ctx;
	t_v2f		range;
	void		(*action)(t_val value, t_val ctx, t_v2f range);
}	t_val_mod;

typedef struct s_perlin_node
{
	const char	*name;
	t_fp_perlin	fp_perlin;
}	t_perlin_node;

extern const t_perlin_node	g_nodes[4];

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

typedef void				(*t_modifier)(t_val_mod *mods,
								t_pdisplay *display, t_perlin *data);

void			perlin_display(void);
void			print_perlin(t_perlin *data);
void			modify(t_pdisplay *display, mlx_key_data_t keydata);
void			call_delta(t_val_mod *fn);
void			draw_perlin(mlx_image_t *img, t_perlin *data,
					t_v3f offset, t_fp_perlin fp_perlin);
bool			is_key_press(mlx_key_data_t keydata, keys_t key);

/* Perlin initiation */
void			init_params(t_pdisplay *display);
bool			init_display(mlx_t *mlx, t_pdisplay *display, t_perlin *data);
void			init_perlin_data(t_perlin *data);

/* Math functions */
void			delta_flt(t_val real, t_val delta, t_v2f range);
void			delta_int(t_val real, t_val delta, t_v2f range);
void			double_flt(t_val real, t_val ctx, t_v2f range);
void			half_flt(t_val real, t_val ctx, t_v2f range);

/* Parameter control */
bool			add_parameter_controls(t_ui *ui, t_ui_element *parent,
					t_pdisplay *display);
void			row_style(t_ui_element *row, bool is_active);

/* Perlin util */
t_ui_element	*find_child_by_type(t_ui_element *parent, t_ui_type type);
void			perlin_key_hook(mlx_key_data_t keydata, void *param);
void			print_perlin(t_perlin *data);

/* Modifier */
t_val_mod		*init_modifiers(t_pdisplay *display, t_perlin *data);

/* Free */
void			cleanup_display(t_pdisplay *display);
#endif
