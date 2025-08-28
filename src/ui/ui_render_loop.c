/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_render_loop.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:03:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/22 11:18:47 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_thread.h"
#include "rt_error.h"

static void	handle_idle_state(t_game *game)
{
	t_ui_context	*ctx;

	if (game->ui && game->ui->context)
	{
		ctx = game->ui->context;
		if (ctx->is_visible && ctx->canvas)
			render_ui(game->ui);
	}
}

static void	handle_render_state(t_game *game)
{
	game->load_screen->ren_prog = (t_v2i){.x = 0, .y = game->img->height};
	ft_memset(game->img->pixels, 0, game->img->width
		* game->img->height * sizeof(uint32_t));
	start_time();
	if (thread_rendering(game->thread_data))
		set_game_state(game, GS_LOAD);
	else
		game->state = GS_ERR;
}

static void	mark_ui_dirty(t_ui_element *ele, t_ui_context *ctx)
{
	t_ui_element	*child;

	if(ele->type == UI_VALUE_BUTTON)
		update_value_label((t_ui_vbtn *)ele->data, ctx);
	child = ele->first_child;
	while (child)
	{
		mark_ui_dirty(child, ctx);
		child = child->next_sibling;
	}
}

static void	handle_load_state(t_game *game)
{
	float	progress;

	pthread_mutex_lock(game->thread_data->progress_lock);
	progress = (float)game->load_screen->ren_prog.x
		/ (float)game->load_screen->ren_prog.y;
	pthread_mutex_unlock(game->thread_data->progress_lock);
	update_load_screen(game->load_screen, game->mlx->delta_time, progress);
	if (progress < 1.0f)
		return ;
	join_threads(game->thread_data->threads, game->thread_data->thread_count);
	end_time();
	set_game_state(game, GS_IDLE);
	mark_ui_dirty(game->ui->root, game->ui->context);
	ui_mark_dirty(game->ui->context);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	render_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->state == GS_IDLE)
		handle_idle_state(game);
	else if (game->state == GS_RENDER)
		handle_render_state(game);
	else if (game->state == GS_LOAD)
		handle_load_state(game);
	else if (game->state == GS_QUIT)
	{
		printf("Exiting...\n");
		cancel_threads(game->thread_data->threads,
			game->thread_data->thread_count);
		mlx_close_window(game->mlx);
	}
	else if (game->state == GS_ERR)
		mlx_close_window(game->mlx);
}
