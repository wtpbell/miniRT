/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:03:43 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 16:22:19 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"



void	free_tokens(char **tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	cleanup_gnl(char *line, int fd)
{
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		free(line);
	}
}

void	del_objects(void *obj)
{
	t_object	*object;

	object = (t_object *)obj;
	if (object)
	{
		if (object->obj)
			free(object->obj);
		free(object);
	}
}

void	del_lights(void *obj)
{
	t_light	*light;

	light = (t_light *)obj;
	if (light)
		free(light);
}
