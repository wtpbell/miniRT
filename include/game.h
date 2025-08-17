/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:01:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/17 16:23:24 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "rt_types.h"
# include "ui.h"
# include "rt_thread.h"

typedef enum	e_game_state
{
	GS_IDLE,
	GS_RENDER,
	GS_LOAD,
	GS_QUIT
}	t_game_state;

struct	s_sample
{
	float		max_depth;
	float		sample_pxl;
};

struct	s_game
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_ui			*ui;
	t_load_screen	*load_screen;
	t_scene			*scene;
	t_sample		*sample;
	t_thread_data	*thread_data;
	t_game_state	state;
};

int	game(t_scene *scene, t_sample *sample);

#endif
