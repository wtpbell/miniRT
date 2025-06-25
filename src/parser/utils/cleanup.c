/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 16:44:01 by bewong            #+#    #+#             */
/*   Updated: 2025/06/25 20:23:58 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "debug/rt_debug.h"

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

void	free_material(void *ptr)
{
	t_mat	*mat;

	mat = (t_mat *)ptr;
	if (mat->name)
	{
		free(mat->name);
		mat->name = NULL;
	}
	if (mat->tex_path)
	{
		free(mat->tex_path);
		mat->tex_path = NULL;
	}
	if (mat->bump_path)
	{
		free(mat->bump_path);
		mat->bump_path = NULL;
	}
	if (mat->bump_map)
	{
		mlx_delete_texture(mat->bump_map);
		mat->bump_map = NULL;
	}
	cleanup_texture(&mat->texture);
	free(ptr);
}

void	cleanup_scene(t_scene *scene)
{
	vector_free(&scene->objects, free);
	vector_free(&scene->lights, free);
	vector_free(&scene->shared_materials, free_material);
}
