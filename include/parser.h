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
	ERR_STOI,
	ERR_STOF,
	ERR_SPHERE_ARGS,
	ERR_COUNT
}	t_error;

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_camera	camera;
}	t_scene;

//* *************************************************************************** //
//                              parser                                      
// *************************************************************************** //

// parser.c
bool		parse_map(t_scene *scene, const char *file);

// camera.c
bool		parse_camera(char **tokens, t_camera *camera);

// light.c
bool		parse_ambient(char **tokens, t_vector *lights);
bool		parse_sphere(char **tokens, t_vector *objects);
void		free_tokens(char **tokens);

// sphere.c
bool		parse_sphere(char **tokens, t_vector *objects);

// parser_error.c
void		exit_err(t_error type, const char *msg);
void		print_err(const char *msg);
t_error		*error(void);
void		cleanup_gnl(char *line, int fd);
void		del_objects(void *obj);
void		del_lights(void *obj);

// type_conversion.c
bool		ft_stoi(const char *s, int *i);
bool		ft_stof(const char *s, float *f);

// utils.c
size_t		token_count(char **tokens);
bool		parse_v3f(t_v3f *v3f, const char *str);
bool		parse_float(float *f, const char *str);
bool		parse_col(t_col32 *col, const char *str);
bool		ft_isspace(int c);

#endif

