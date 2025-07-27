/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:01:00 by bewong            #+#    #+#             */
/*   Updated: 2025/07/27 21:03:41 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "MLX42/MLX42.h"
# include "scene.h"

/* Forward declarations */
typedef struct s_ui t_ui;

typedef struct	s_game
{
    mlx_t		*mlx;
    mlx_image_t	*img;
    t_ui		*ui;
    t_scene		*scene;
    bool		needs_redraw;
    bool		should_exit;
} t_game;

#endif
