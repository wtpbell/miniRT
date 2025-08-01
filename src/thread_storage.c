/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_storage.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/01 17:10:11 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/01 18:42:32 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_thread.h"
#include <stdio.h>

static pthread_key_t	g_thread_index_key;

int	create_index_key(void)
{
	return (pthread_key_create(&g_thread_index_key, NULL));
}

int	delete_index_key(void)
{
	return (pthread_key_delete(g_thread_index_key));
}

int	get_thread_index(void)
{
	void	*ptr;

	ptr = pthread_getspecific(g_thread_index_key);
	if (ptr != NULL)
		return (*(int *)ptr);
	perror("minirt");
	return (0);
}

int	set_thread_index(void *i)
{
	return (pthread_setspecific(g_thread_index_key, i));
}
