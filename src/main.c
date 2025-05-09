/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:21:05 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/09 17:38:18 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parser.h"

static bool	valid_file_format(const char *file)
{
	const size_t	len = ft_strlen(file);

	if (len < 4)
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
	
	if (!valid_input(argc, argv))
		print_err("error");
	if (!parse_map(&scene, argv[1]))
		exit_err(*error(), "Parsing error");
	return (0);
}
