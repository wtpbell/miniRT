/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 11:37:50 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/19 18:35:37 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "ray.h"
# include "color.h"
# include "vector.h"

typedef struct s_ui
{
	t_obj	*curr_obj;
	int		curr_i;
	int		prev_i;
	int		refresh;
} t_ui;

typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*ui_layer;
	t_scene		*scene;
	t_ui		ui;
} t_game;

int				game(t_scene *scene);
void			render(t_scene *scene);
int				sphere_intersect(t_obj *obj, t_ray *ray, float *dst);
mlx_image_t*	rt_put_string(mlx_image_t *strimage, const char* str, int32_t x, int32_t y);

#endif
