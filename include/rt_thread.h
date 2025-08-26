/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_thread.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/03 17:35:16 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/26 15:53:08 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_THREAD_H
# define RT_THREAD_H

# include <pthread.h>
# include "rt_types.h"
# include "MLX42/MLX42.h"
# include "scene.h"
# include "game.h"

# define THRD_CNT	20

typedef enum e_thread_state
{
	THRD_IDLE,
	THRD_WORKING,
	THRD_COMPLETE,
	THRD_CANCELED
}	t_thrd_state;

struct s_thread_data
{
	t_scene			*scene;
	t_sample		*sample;
	int				thread_count;
	t_pthread_instr	*threads;
	int				*progress_count;
	pthread_mutex_t	*progress_lock;
};

struct s_pthread_instr
{
	int				i;
	t_thrd_state	state;
	uint32_t		start_y;
	uint32_t		end_y;
	mlx_image_t		*img;
	pthread_t		thread;
	t_thread_data	*shared_data;
};

/* threads.c */

bool			thread_rendering(t_thread_data *data);
void			join_threads(t_pthread_instr *instr, int count);
void			cancel_threads(t_pthread_instr *instr, int count);

/* thread_data.c */

t_thread_data	*init_thread_data(int thread_count, t_game *game);
void			cleanup_thread_data(t_thread_data *thread_data);

/* benchmark.c */

void			start_time(void);
void			end_time(void);

#endif
