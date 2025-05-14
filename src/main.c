/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:21:05 by jboon             #+#    #+#             */
/*   Updated: 2025/05/14 15:13:22 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	main(int argc, char **argv)
{
	t_scene	scene;

	ft_bzero(&scene, sizeof(t_scene));
	if (!valid_input(argc, argv))
	{
		ft_putstr_fd("Error\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (!parse_map(&scene, argv[1]))
		print_error(ERR_PARSE_FAIL, "map", argv[1]);
	cleanup_scene(&scene);
	exit(EXIT_FAILURE);
}
