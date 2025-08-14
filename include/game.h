/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:01:00 by bewong            #+#    #+#             */
/*   Updated: 2025/08/14 22:04:11 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "MLX42/MLX42.h"
# include "scene.h"
# include "rt_types.h"
# include "ui.h"

struct	s_sample
{
	float		max_depth;
	float		sample_pxl;
};

struct	s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_ui		*ui;
	t_scene		*scene;
	t_sample	*sample;
	bool		needs_redraw;
	bool		should_exit;
};

int	game(t_scene *scene, t_sample *sample);

#endif
