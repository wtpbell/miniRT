/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_thread.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/03 17:35:16 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/01 18:54:16 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_THREAD_H
# define RT_THREAD_H

# include <pthread.h>
# include "MLX42/MLX42.h"
# include "scene.h"

# define THRD_CNT	4

typedef struct s_pthread_instr
{
	int			i;
	uint32_t	start_y;
	uint32_t	end_y;
	t_scene		*scene;
	mlx_image_t	*img;
	pthread_t	thread;
}	t_pthread_instr;

/* threads.c */

bool	thread_rendering(t_scene *scene);

/* thread_utils.c */

int		create_index_key(void);
int		delete_index_key(void);
int		get_thread_index(void);
int		set_thread_index(void *i);

/* benchmark.c */

void	start_time(void);
void	end_time(void);

#endif
