/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/11 17:52:35 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 23:00:58 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "rt_thread.h"
#include "minirt.h"

static bool	init_thread(t_pthread_instr *instr, uint32_t start, uint32_t end)
{
	instr->state = THRD_WORKING;
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
		if (instr[i].state == THRD_WORKING
			&& pthread_cancel(instr[i].thread) == 0)
		{
			instr[i].state = THRD_CANCELED;
			pthread_join(instr[i].thread, NULL);
		}
		++i;
	}
}

void	join_threads(t_pthread_instr *instr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (instr[i].state == THRD_WORKING)
		{
			instr[i].state = THRD_COMPLETE;
			pthread_join(instr[i].thread, NULL);
		}
		++i;
	}
}

bool	thread_rendering(t_thread_data *data)
{
	if (!create_threads(data, data->thread_count))
		return (perror("minirt"), false);
	return (true);
}
