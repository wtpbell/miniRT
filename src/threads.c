/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:52:35 by jboon             #+#    #+#             */
/*   Updated: 2025/08/13 19:00:45 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "rt_thread.h"
#include "minirt.h"

static bool	init_thread(t_pthread_instr *instr, t_scene *scene, uint32_t start,
	uint32_t end)
{
	instr->scene = scene;
	instr->start_y = start;
	instr->end_y = end;
	instr->img = scene->camera.img_plane;
	return (pthread_create(&instr->thread, NULL, render, instr) == 0);
}

static void	cancel_threads(t_pthread_instr *instr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_cancel(instr[i].thread);
		pthread_join(instr[i].thread, NULL);
		++i;
	}
}

static void	join_threads(t_pthread_instr *instr, int count)
{
	int	i;

	i = 0;
	while (i < count)
		pthread_join(instr[i++].thread, NULL);
}

static bool	create_threads(t_pthread_instr *instr,
		int count, t_scene *scene, t_sample *sample)
{
	uint32_t	delta;
	uint32_t	start_y;
	uint32_t	end_y;
	int			i;

	i = 0;
	start_y = 0;
	delta = scene->camera.img_plane->height / count;
	while (i < count)
	{
		end_y = start_y + delta;
		if (i == (count - 1) || end_y > scene->camera.img_plane->height)
			end_y = scene->camera.img_plane->height;
		instr[i].i = i;
		instr[i].sample = sample;
		if (!init_thread(&instr[i], scene, start_y, end_y))
			return (cancel_threads(instr, i), false);
		start_y = end_y;
		++i;
	}
	return (true);
}

bool	thread_rendering(t_scene *scene, t_sample *sample)
{
	t_pthread_instr	instr[THRD_CNT];

	start_time();
	if (!create_threads(instr, THRD_CNT, scene, sample))
		return (perror("minirt"), end_time(), false);
	join_threads(instr, THRD_CNT);
	end_time();
	return (true);
}
