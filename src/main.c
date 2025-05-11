/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:21:05 by jboon             #+#    #+#             */
/*   Updated: 2025/05/10 11:29:55 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parser.h"

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
		*error() = ERR_NUM_ARGS;
		return (false);
	}
	if (!valid_file_format(argv[1]))
	{
		*error() = ERR_FILE_FORMAT;
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_scene	scene;
	int		return_code;
	
	if (!valid_input(argc, argv))
	{
		printf("Error\n");
		print_err(NULL);
		return (1);
	}
	if (!parse_map(&scene, argv[1]))
	{
		printf("Error\n");
		print_err(NULL);
		return (1);
	}

	// Do your rendering or other operations here
	return_code = 0;

	// Cleanup
	vector_free(&scene.objects, del_objects);
	vector_free(&scene.lights, del_lights);
	return (return_code);
}
