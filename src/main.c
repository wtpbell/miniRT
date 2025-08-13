/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:21:05 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 16:33:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minirt.h"

void	perlin_display(void);

static bool	valid_file_format(const char *file)
{
	const size_t	len = ft_strlen(file);

	if (len < 3)
		return (false);
	file += len - 3;
	return (ft_strncmp(file, ".rt", 3) == 0);
}

static bool	valid_input(int argc, char **argv)
{
	if (argc != 2)
	{
		print_error(ERR_NUM_ARGS, "input", NULL);
		return (false);
	}
	if (!valid_file_format(argv[1]))
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
		|| !create_default_materials(&scene->shared_materials))
	{
		perror("init_scene_and_vector");
		cleanup_scene(scene);
		return ;
	}
}

int	main(int argc, char **argv)
{
	t_scene	scene;

	if (1)
	{
		perlin_display();
		return (EXIT_SUCCESS);
	}

	ft_bzero(&scene, sizeof(t_scene));
	if (!valid_input(argc, argv))
		return (EXIT_FAILURE);
	init_scene_and_vector(&scene);
	if (!parse_map(&scene, argv[1]))
		return (print_error(ERR_PARSE_FAIL, "map", argv[1]), EXIT_FAILURE);
	else if (!validate_scene(&scene))
		return (cleanup_scene(&scene), EXIT_FAILURE);
	game(&scene);
	cleanup_scene(&scene);
	return (EXIT_SUCCESS);
}
