/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 22:20:50 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 17:20:25 by bewong        ########   odam.nl         */
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

#define MAX_POS			10000.0f
#define MAX_RADIUS		1000.0f
#define MAX_BRIGHTNESS	1000.0f
#define MIN_RADIUS		0.0f

// typedef bool (*t_parser)(char **, t_scene *);

typedef enum e_error
{
	ERR_NONE,
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
	ERR_TOKEN_COUNT,
	ERR_DUPLICATE,
	ERR_INVALID_VALUE,
	ERR_V3F,
	ERR_POSITIVE_VALUE,
	ERR_COUNT
}	t_error;

typedef struct s_scene
{
	t_vector	objects;
	t_vector	lights;
	t_camera	camera;
}	t_scene;

//* *************************************************************************** //
//                              Core Parser                                      

// Token utilities
bool	validate_tokens(const char *first_token, const char *line);
char	*get_first_token(const char *str);
size_t	get_expected_token_count(const char *type);

// Sphere parsing
bool	parse_sphere(char **tokens, t_scene *scene);
// *************************************************************************** //

// core/parser.c
bool	parse_line(t_scene *scene, char *line);

// core/file_parser.c
bool	parse_map(t_scene *scene, const char *file);

// core/element_parser.c
bool	parse_scene_element(const char *type, char **tokens, t_scene *scene);
bool	has_duplicate_identifier(const char *type, t_scene *scene);

//* *************************************************************************** //
//                              Objects                                      
// *************************************************************************** //

// objects/sphere.c
bool	parse_sphere_object(char **tokens, t_object **objects);
t_sphere	*create_sphere(t_v3f pos, float diameter, t_col32 color);

//* *************************************************************************** //
//                              Utils                                      
// *************************************************************************** //

// utils/string_utils.c
char	*trim_whitespace(const char *str);
char	*clean_spaces(const char *str);
size_t	count_tokens_in_str(const char *str);
// size_t	get_expected_token_count(const char *type);

// utils/vector_utils.c
size_t	count_vector_components(const char *str);
bool	parse_v3f(t_v3f *v3f, const char *str);
bool	parse_col(t_col32 *col, const char *str);
bool	parse_float(const char *str, float *out);

// utils/string_to_num.c
bool	ft_stof(const char *s, float *f);
bool	ft_stoi(const char *s, int *i);

// utils/error.c
t_error	*error(void);
void	exit_err(t_error type, const char *msg);
bool	print_err(const char *msg);
void	cleanup_gnl(char *line, int fd);

// Common utils
void	free_tokens(char **tokens);

// utils/cleanup.c
void	del_objects(void *obj);
void	del_lights(void *light);

#endif
