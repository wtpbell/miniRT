/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/31 20:43:04 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/31 21:34:44 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"

static inline bool	is_key_down(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key
		&& (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT));
}

void	quit_on_escape(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (is_key_down(keydata, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}
