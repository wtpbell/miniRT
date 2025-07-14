/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/11 17:52:35 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/14 15:53:32 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_thread.h"
#include "minirt.h"

#include "debug/rt_debug.h" //                                                        remove me:)

static bool init_thread(t_pthread_instr *instr, t_scene *scene, uint32_t start, uint32_t end)
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
	while(i < count)
		pthread_join(instr[i++].thread, NULL);
}

bool	thread_rendering(t_scene *scene)
{
	t_pthread_instr	instr[THRD_CNT];
	uint32_t		delta;
	uint32_t		start_y;
	uint32_t		end_y;
	int				i;

	i = 0;
	start_y = 0;
	delta = scene->camera.img_plane->height / THRD_CNT;
	start_time();
	while (i < THRD_CNT)
	{
		end_y = start_y + delta;
		if (i == THRD_CNT - 1 || end_y > scene->camera.img_plane->height)
			end_y = scene->camera.img_plane->height;
		if (!init_thread(&instr[i], scene, start_y, end_y))
			return (perror("minirt:"), end_time(), cancel_threads(instr, i), false);
		start_y = end_y;
		++i;
	}
	join_threads(instr, i);
	end_time();
	return (true);
}
