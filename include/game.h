/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:01:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/22 14:59:23 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "rt_types.h"
# include "ui.h"
# include "rt_thread.h"

typedef enum e_game_state
{
	GS_IDLE = 0,
	GS_RENDER = 1,
	GS_LOAD = 2,
	GS_QUIT
}	t_game_state;

struct s_sample
{
	float	max_depth;
	float	sample_pxl;
};

struct s_game
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

int		game(t_scene *scene, t_sample *sample);
void	set_game_state(t_game *game, t_game_state state);
void	key_hook(mlx_key_data_t keydata, void *param);
void	mouse_hook(mouse_key_t button, action_t action,
			__attribute__((unused)) modifier_key_t mods, void *param);

#endif
