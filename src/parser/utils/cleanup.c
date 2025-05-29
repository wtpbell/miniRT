/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:44:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/29 14:54:23 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_tokens(char **tokens)
{
	size_t	i;

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

static void	free_obj(void *ptr)
{
	t_obj	*obj;

	obj = (t_obj *)ptr;
	if (obj->r.mat)
		free(obj->r.mat);
	free(obj);
}

void	cleanup_scene(t_scene *scene)
{
	vector_free(&scene->objects, free_obj);
	vector_free(&scene->lights, free);
}
