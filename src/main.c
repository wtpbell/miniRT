/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:21:05 by jboon             #+#    #+#             */
/*   Updated: 2025/07/23 21:20:00 by bewong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

bool	valid_file_format(const char *file, const char *ext)
{
	const size_t	len = ft_strlen(file);
	const size_t	ext_len = ft_strlen(ext);

	if (len < ext_len)
		return (false);
	file += len - ext_len;
	return (ft_strncmp(file, ext, 3) == 0);
}

static bool	valid_input(int argc, char **argv)
{
	if (argc != 2)
	{
		print_error(ERR_NUM_ARGS, "input", NULL);
		return (false);
	}
	if (!valid_file_format(argv[1], ".rt"))
	{
		print_error(ERR_FILE_FORMAT, "format", argv[1]);
		return (false);
	}
	return (true);
}

static void	init_scene_and_vector(t_scene *scene)
{
	ft_bzero(scene, sizeof(t_scene));
	if (!vector_init(&scene->objects, 8) || !vector_init(&scene->lights, 8)
		|| !vector_init(&scene->shared_materials, 8)
		|| !vector_init(&scene->shared_mesh, 8)
		|| !create_default_materials(&scene->shared_materials))
	{
		perror("init_scene_and_vector");
		cleanup_scene(scene);
		return ;
	}
}

static void	init_sample(t_sample *sample)
{
	sample->max_depth = 1.0f;
	sample->sample_pxl = 2.0f;
}

int	main(int argc, char **argv)
{
	t_scene		scene;
	t_sample	*sample;

	ft_bzero(&scene, sizeof(t_scene));
	if (!valid_input(argc, argv))
		return (EXIT_FAILURE);
	init_scene_and_vector(&scene);
	if (!parse_map(&scene, argv[1]))
		return (print_error(ERR_PARSE_FAIL, "map", argv[1]), EXIT_FAILURE);
	else if (!validate_scene(&scene))
		return (cleanup_scene(&scene), EXIT_FAILURE);
	sample = ft_calloc(1, sizeof(t_sample));
	if (!sample)
		return (cleanup_scene(&scene), EXIT_FAILURE);
	init_sample(sample);
	game(&scene, sample);
	cleanup_scene(&scene);
	free(sample);
	return (EXIT_SUCCESS);
}
