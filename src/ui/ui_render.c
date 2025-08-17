/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_render.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:03:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/17 17:05:53 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "rt_thread.h"

uint32_t	blend_colors(uint32_t bg, uint32_t fg)
{
	float	alpha;
	float	inv_alpha;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	// I thought alpha is stored at 0xFF not 0xFF000000
	alpha = ((fg >> 24) & 0xFF) / 255.0f;
	inv_alpha = 1.0f - alpha;
	r = (uint8_t)(((bg >> 16) & 0xFF) * inv_alpha
			+ ((fg >> 16) & 0xFF) * alpha);
	g = (uint8_t)(((bg >> 8) & 0xFF) * inv_alpha
			+ ((fg >> 8) & 0xFF) * alpha);
	b = (uint8_t)((bg & 0xFF) * inv_alpha + (fg & 0xFF) * alpha);
	return ((0xFFu << 24) | (r << 16) | (g << 8) | b);
}

void	render_ui_element(t_ui_element *e, t_ui_context *c)
{
	t_ui_element	*child;

	if (!e->visible)
		return ;
	e->abs_pos = e->pos;
	if (e->parent)
		e->abs_pos = v2f_add(e->parent->abs_pos, e->pos);
	if (e->type == UI_PANEL || e->type == UI_HEADER)
	{
		draw_rect(c->canvas, e->abs_pos, e->size, e->style.bg_color);
		draw_rect_border(c->canvas, e->abs_pos, e->size, e->style.border_color);
	}
	else if (e->type == UI_BUTTON)
		draw_button(e, c);
	else if (e->type == UI_LABEL)
		draw_text(c->canvas, ((t_ui_label *)e->data)->text, e->abs_pos,
			e->style.text_color);
	child = e->first_child;
	while (child && (render_ui_element(child, c), 1))
		child = child->next_sibling;
}

void	render_loop(void *param)
{
	// TODO: file render_loop.c does not contain the function render_loop
	t_game			*game;
	t_ui_context	*ctx;
	float			progress;

	game = (t_game *)param;
	if (game->state == GS_IDLE)
	{
		if (game->ui && game->ui->context)
		{
			ctx = game->ui->context;
			if (ctx->is_visible && ctx->canvas)
				render_ui(game->ui);
		}
	}
	else if (game->state == GS_RENDER)
	{
		game->load_screen->ren_prog = (t_v2i){.x = 0, .y = game->img->height};
		ft_memset(game->img->pixels, 0, game->img->width
			* game->img->height * sizeof(uint32_t));
		
		start_time();
		if (thread_rendering(game->thread_data))
		{
			game->state = GS_LOAD;
			game->img->enabled = false;
			game->ui->context->canvas->enabled = false;
			game->load_screen->bg.img->enabled = true;
		}
		else
		{
			write(2, "Failed to create threads! Try again.\n", 38);
			game->state = GS_IDLE;
		}
	}
	else if (game->state == GS_LOAD)
	{
		pthread_mutex_lock(game->thread_data->progress_lock);
		progress = (float)game->load_screen->ren_prog.x / (float)game->load_screen->ren_prog.y;
		pthread_mutex_unlock(game->thread_data->progress_lock);

		printf("\x1b[1F\x1b[2K");
		printf("RENDERING: %.2f%%\n", progress * 100.0f);

		update_load_screen(game->load_screen, game->mlx->delta_time, progress);

		if (progress >= 1.0f)
		{
			join_threads(game->thread_data->threads, game->thread_data->thread_count);
			
			game->state = GS_IDLE;
			game->img->enabled = true;
			game->ui->context->canvas->enabled = true;
			game->load_screen->bg.img->enabled = false;
			end_time();
			printf("\n");
		}
	}
	else if (game->state == GS_QUIT)
	{
		printf("Exiting...\n");
		// TODO: Should not cancel if threads have already been joined (happens when rendering is complete)
		cancel_threads(game->thread_data->threads, game->thread_data->thread_count);
		printf("Closing window\n");
		mlx_close_window(game->mlx);
	}
}
