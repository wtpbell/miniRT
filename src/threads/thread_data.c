/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_data.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/17 21:16:45 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/17 21:17:51 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_thread.h"

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
