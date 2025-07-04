/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_thread.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/03 17:35:16 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/03 17:59:57 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_THREAD_H
# define RT_THREAD_H

# include <pthread.h>
# include "MLX42/MLX42.h"
# include "scene.h"

typedef struct s_pthread_instr
{
	uint32_t	start_y;
	uint32_t	end_y;
	t_scene		*scene;
	mlx_image_t	*img;
	pthread_t	thread;
} t_pthread_instr;

#endif
