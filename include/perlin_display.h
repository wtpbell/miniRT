/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin_display.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 11:39:26 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/01 13:51:04 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_DISPLAY_H
# define PERLIN_DISPLAY_H

# include "MLX42/MLX42.h"
# include "rt_math.h"
# include "material.h"

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
	void		(*print)(t_val value, const char *name);
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
}	t_pdisplay;

void	perlin_display(void);

#endif
