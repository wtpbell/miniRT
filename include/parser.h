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

typedef enum e_error
{
	ERR_NO,
	ERR_NUM_ARGS,
	ERR_FILE_FORMAT,
	ERR_RANGE,
	ERR_MEM,
	ERR_FILE_READ,
	ERR_UNKNOWN_TOKEN,
	ERR_COUNT
}	t_error;

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_camera	camera;
	t_light		ambient;
}	t_scene;



void		exit_err(t_error type, const char *msg);
void		print_err(const char *msg);
t_error		*error(void);

#endif // PARSER_H

