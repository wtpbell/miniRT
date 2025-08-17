/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/11 17:52:35 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 16:56:28 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "rt_thread.h"
#include "minirt.h"

static bool	init_thread(t_pthread_instr *instr, uint32_t start, uint32_t end)
{
	instr->start_y = start;
	instr->end_y = end;
	instr->img = instr->shared_data->scene->camera.img_plane;
	return (pthread_create(&instr->thread, NULL, render, instr) == 0);
}

static bool	create_threads(t_thread_data *data, int thread_count)
{
	t_scene		*scene;
	uint32_t	delta;
	uint32_t	start_y;
	uint32_t	end_y;
	int			i;

	i = 0;
	start_y = 0;
	scene = data->scene;
	delta = scene->camera.img_plane->height / thread_count;
	while (i < thread_count)
	{
		end_y = start_y + delta;
		if (i == (thread_count - 1) || end_y > scene->camera.img_plane->height)
			end_y = scene->camera.img_plane->height;
		data->threads[i].i = i;
		data->threads[i].shared_data = data;
		if (!init_thread(data->threads + i, start_y, end_y))
			return (cancel_threads(data->threads, i), false);
		start_y = end_y;
		++i;
	}
	return (true);
}

void	cancel_threads(t_pthread_instr *instr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		printf("LETS GO %d!\n", i);
		pthread_cancel(instr[i].thread);
		pthread_join(instr[i].thread, NULL);
		++i;
	}
}

void	join_threads(t_pthread_instr *instr, int count)
{
	int	i;

	i = 0;
	while (i < count)
		pthread_join(instr[i++].thread, NULL);
}

void	cleanup_thread_data(t_thread_data *thread_data)
{
	free(thread_data->threads);
	if (thread_data->progress_lock != NULL)
		pthread_mutex_destroy(thread_data->progress_lock);
	free(thread_data->progress_lock);
	free(thread_data);
}

t_thread_data	*init_thread_data(int thread_count, t_game *game)
{
	t_thread_data	*thread_data;

	thread_data = ft_calloc(1, sizeof(t_thread_data));
	if (thread_data == NULL)
		return (NULL);

	thread_data->thread_count = thread_count;
	thread_data->threads = ft_calloc(thread_count, sizeof(t_pthread_instr));
	if (thread_data->threads == NULL)
		return (cleanup_thread_data(thread_data), NULL);

	thread_data->progress_lock = malloc(sizeof(pthread_mutex_t));
	if (thread_data->progress_lock == NULL)
		return (cleanup_thread_data(thread_data), NULL);
	if (pthread_mutex_init(thread_data->progress_lock, NULL) != 0)
	{
		free(thread_data->progress_lock);
		thread_data->progress_lock = NULL;
		return (cleanup_thread_data(thread_data), NULL);
	}

	thread_data->progress_count = &game->load_screen->ren_prog.x;
	thread_data->sample = game->sample;
	thread_data->scene = game->scene;
	return (thread_data);
}

bool	thread_rendering(t_thread_data *data)
{
	if (!create_threads(data, data->thread_count))
		return (perror("minirt"), false);
	return (true);
}
