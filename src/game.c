/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/16 11:50:39 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/19 18:39:24 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "minirt.h"
#include "debug/rt_debug.h"

#include <stdio.h>

#define WIDTH	1024
#define HEIGHT	1024

static void	clear_image(mlx_image_t *img, t_col32 col)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, col);
			++x;
		}
		++y;
	}
}

static inline bool	is_key_down(mlx_key_data_t keydata, keys_t key, action_t action)
{
	return (keydata.key == key
		&& (keydata.action == MLX_PRESS || keydata.action == action));
}

static const char *type_to_str(t_obj_type type)
{
	if (type == OBJ_SPHERE)
		return ("SPHERE");
	else if (type == OBJ_CYLINDER)
		return ("CYLINDER");
	return ("NOTHING");
}

// TODO: Implementation needed ()
static void	update(mlx_key_data_t keydata, void *ctx)
{
	t_game	*game;

	game = (t_game *)(ctx);
	if (is_key_down(keydata, MLX_KEY_UP, MLX_PRESS))
	{
		if (game->ui.curr_i < game->scene->objects.size)
			++game->ui.curr_i;
		game->ui.curr_obj = vector_get(&game->scene->objects, game->ui.curr_i);
		game->ui.refresh = 1;
	}
	else if(is_key_down(keydata, MLX_KEY_DOWN, MLX_PRESS))
	{
		if (game->ui.curr_i > 0)
			--game->ui.curr_i;
		game->ui.curr_obj = vector_get(&game->scene->objects, game->ui.curr_i);
		game->ui.refresh = 1;
	}
	if (game->ui.curr_obj != NULL && game->ui.refresh == 1)
	{
		clear_image(game->ui_layer, C_TRANS);
		rt_put_string(game->ui_layer, type_to_str(game->ui.curr_obj->type + game->ui.curr_i), 0, 0);
	}

}

void	cam_to_world(t_mat4x4 mat, t_v3f pos, t_v3f dir, t_v3f up)
{
	t_v3f	x_axis;
	t_v3f	y_axis;

	id_m4x4(mat);
	trans_m4x4(mat, pos);
	x_axis = v3f_norm(v3f_cross(up, dir));
	y_axis = v3f_norm(v3f_cross(dir, x_axis));
	x_axis = v3f_scale(x_axis, -1);
	mat[0] = x_axis.x;
	mat[1] = x_axis.y;
	mat[2] = x_axis.z;
	mat[4] = y_axis.x;
	mat[5] = y_axis.y;
	mat[6] = y_axis.z;
	mat[8] = dir.x;
	mat[9] = dir.y;
	mat[10] = dir.z;
}

static bool	cam_init(t_cam *cam, mlx_t *mlx)
{
	cam->img_plane = mlx_new_image(mlx, mlx->width, mlx->height);
	if (cam->img_plane == NULL
		|| mlx_image_to_window(mlx, cam->img_plane, 0, 0) == -1)
		return (false);
	cam->aspect_ratio = cam->img_plane->width / (float)cam->img_plane->height;
	cam->bg_col = init_col32(127, 0, 127, 255);
	cam->t.dir = v3f_norm(cam->t.dir);
	cam_to_world(cam->cam_to_world, cam->t.pos, cam->t.dir, init_v3f(0, 1, 0));
	mlx_set_instance_depth(cam->img_plane->instances, 1);
	return (true);
}

static bool	game_init(t_game *game, t_scene *scene)
{
	game->scene = scene;
	game->mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (game->mlx == NULL)
		return (false);
	game->ui_layer = mlx_new_image(game->mlx, game->mlx->width, game->mlx->height);
	if (game->ui_layer == NULL || mlx_image_to_window(game->mlx, game->ui_layer, 0, 0) == -1)
		return (false);
	clear_image(game->ui_layer, C_TRANS);
	rt_put_string(game->ui_layer, "Hello World!", 100, 100);
	rt_put_string(game->ui_layer, "Hello World!", 10, 10);
	rt_put_string(game->ui_layer, "Hello World!", 200, 200);
	mlx_set_instance_depth(game->ui_layer->instances, 2);

	return (true);
}

int	game(t_scene *scene)
{
	t_game	game;

	ft_bzero(&game, sizeof(t_game));
	if (game_init(&game, scene) && cam_init(&scene->camera, game.mlx))
	{
		printf("HAY?\n");
		scene_print(scene);
		// mlx_loop_hook(game.mlx, update, &game);
		mlx_key_hook(game.mlx, update, &game);
		render(scene);
		mlx_loop(game.mlx);
	}
	mlx_terminate(game.mlx);
	return (0);
}
