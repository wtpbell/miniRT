/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 10:07:36 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 16:42:42 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINER_H
# define CONTAINER_H

# include <stdbool.h>

# define COL_INIT_CAPACITY 10

typedef struct s_vector
{
	void	**items;
	int		size;
	int		capacity;
}	t_vector;

bool			vector_init(t_vector *vec, int capacity);
bool			vector_add(t_vector *vec, void *item);
void			vector_set(t_vector *vec, int i, void *item);
void			*vector_get(t_vector *vec, int i);
bool			vector_rm(t_vector *vec, int i, void (*del)(void *));
void			vector_free(t_vector *vec, void (*del)(void *));
void			*vector_find(t_vector *vec, void *ctx,
					int (*fn)(int i, void *item, void *ctx));

#endif
