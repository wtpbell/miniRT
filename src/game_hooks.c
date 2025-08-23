/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game_hooks.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/17 22:37:13 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 22:50:31 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static inline bool	is_key_down(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key
		&& (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT));
}

static inline bool	is_mouse_press(mouse_key_t mouse_input,
	mouse_key_t mouse_key, action_t mouse_action)
{
	return (mouse_input == mouse_key && mouse_action == MLX_PRESS);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (is_key_down(keydata, MLX_KEY_H) && game->ui)
		toggle_ui_visibility(game->ui);
	if (is_key_down(keydata, MLX_KEY_ESCAPE))
		game->state = GS_QUIT;
}

void	mouse_hook(mouse_key_t button, action_t action,
	__attribute__((unused)) modifier_key_t mods, void *param)
{
	t_game	*game;
	int32_t	x;
	int32_t	y;

	game = (t_game *)param;
	if (is_mouse_press(button, MLX_MOUSE_BUTTON_LEFT, action)
		&& game->state == GS_IDLE)
	{
		if (game->ui && game->ui->context && game->ui->context->is_visible)
		{
			mlx_get_mouse_pos(game->mlx, &x, &y);
			handle_ui_click(game->ui->root, x, y, game->ui->context);
		}
	}
}
