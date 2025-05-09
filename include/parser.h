/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:20:50 by bewong            #+#    #+#             */
/*   Updated: 2025/05/08 22:20:50 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "container.h"

typedef enum e_error
{
	ERR_NO,
	ERR_NUM_ARGS,
	ERR_FILE_FORMAT,
	ERR_RANGE,
	ERR_MEM,
	ERR_FILE_READ,
	ERR_UNKNOWN_TOKEN,
	ERR_FILE_NONEXIST,
	ERR_COUNT
}	t_error;

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_camera	camera;
}	t_scene;


bool		parse_map(t_scene *scene, const char *file);

bool		parse_camera(char **tokens, t_camera *camera);
bool		parse_ambient(char **tokens, t_vector *lights);
void		free_tokens(char **tokens);

void		exit_err(t_error type, const char *msg);
void		print_err(const char *msg);
t_error		*error(void);

#endif // PARSER_H

