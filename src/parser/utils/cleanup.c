/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:44:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 17:43:21 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	del_objects(void *obj)
{
	t_object	*object;

	if (!obj)
		return ;
	object = (t_object *)obj;
	free(object->obj);
	free(object);
}

void	del_lights(void *light)
{
	if (!light)
		return ;
	free(light);
}

void	free_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}


void	cleanup_gnl(char *line, int fd)
{
	if (line)
		free(line);
	close(fd);
	get_next_line(-1);
}
