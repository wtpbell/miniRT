/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 22:20:50 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/15 16:56:55 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stddef.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include "scene.h"
# include "container.h"
# include "vector.h"

# define MAX_POS			10000.0f
# define MAX_RADIUS			1000.0f
# define MIN_RADIUS			0.0f
# define MAX_BRIGHTNESS		1000.0f
# define MAX_COLOR			255
# define MIN_COLOR			0
# define MAX_FOV			180.0f
# define MIN_FOV			0.0f
# define MAX_LIGHT_RATIO	1.0f
# define MIN_LIGHT_RATIO	0.0f

# define RED 			"\033[31m"
# define GREEN 			"\033[32m"
# define YELLOW 		"\033[33m"
# define BLUE 			"\033[34m"
# define RESET 			"\033[0m"

typedef bool			(*t_parser)(char **, t_scene *);

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
	ERR_PARSE_FAIL,
	ERR_INF,
	ERR_MISSING_COMPONENT,
	ERR_COUNT
}	t_error;

// Token utilities
bool		validate_tokens(const char *first_token, const char *line);
char		*get_first_token(const char *str);
size_t		get_expected_token_count(const char *type);

/* ---------------------Core--------------------- */
// parser.c
bool		parse_line(t_scene *scene, char *line);

// file_parser.c
bool		parse_map(t_scene *scene, const char *file);

// element_parser.c
bool		parse_scene_element(const char *type,
				char **tokens, t_scene *scene);

/* ---------------------Elements--------------------- */
// camera.c
bool		parse_camera(char **tokens, t_scene *scene);
// light.c
bool		parse_light(char **tokens, t_scene *scene);

/* ---------------------Objects--------------------- */
// sphere.c
bool		parse_sphere(char **tokens, t_scene *scene);
bool		parse_diameter(float *out, const char *str);
// plane.c
bool		parse_plane(char **tokens, t_scene *scene);
// cylinder.c
bool		parse_cylinder(char **tokens, t_scene *scene);

/* ---------------------Utils--------------------- */
// string_utils.c
void		clean_spaces(char *str);
size_t		token_count_in_str(const char *str);

// vector_utils.c
bool		parse_v3f(t_v3f *v3f, const char *str);
bool		parse_col(t_col32 *col, const char *str);
bool		parse_dir(t_v3f *dir, const char *str);

// general_utils.c
size_t		token_count(char **tokens);
size_t		count_vector_components(const char *str);

// string_to_num.c
bool		ft_stof(const char *s, float *f);
bool		ft_stoi(const char *s, int *i);

// validate_utils.c
bool		validate_scene(const t_scene *scene);
bool		validate_scene(const t_scene *scene);
bool		split_and_validate(const char *str, char ***out_tokens,
				size_t expected_count, const char *ctx);
bool		parse_and_validate_float(float *out, const char *str,
				t_v2f range, const char *token);

// error.c
void		print_error(t_error type, const char *ctx, const char *value);
void		exit_err(t_error type, const char *ctx, const char *value);

// cleanup.c
void		del_objects(void *obj);
void		del_lights(void *light);
void		free_tokens(char **tokens);
void		cleanup_gnl(char *line, int fd);
void		cleanup_scene(t_scene *scene);
#endif
