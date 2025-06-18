/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 22:20:50 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/18 16:58:21 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <stddef.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include "minirt.h"
# include "scene.h"
# include "container.h"
# include "vector.h"
# include "get_next_line.h"

# define MAX_POS			10000.0f
# define MAX_RADIUS			1000.0f
# define MIN_RADIUS			0.0f
# define MAX_HEIGHT			1000.0f
# define MIN_HEIGHT			0.0f
# define MAX_BRIGHTNESS		1000.0f
# define MAX_COLOR			255
# define MIN_COLOR			0
# define MAX_FOV			180.0f
# define MIN_FOV			0.0f
# define MAX_LIGHT_RATIO	1.0f
# define MIN_LIGHT_RATIO	0.0f
# define MAX_DIR			1.0f
# define MIN_DIR			-1.0f

# define RED 			"\033[31m"
# define GREEN 			"\033[32m"
# define YELLOW 		"\033[33m"
# define BLUE 			"\033[34m"
# define RESET 			"\033[0m"

typedef bool			(*t_parser)(char **, t_scene *);
typedef bool			(*t_conv_to_enum)(int *val, const void *raw);

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
	ERR_FORMAT,
	ERR_INV_MAT_NAME,
	ERR_UNKNOWN_MAT_TYPE,
	ERR_UNKNOWN_FIELD,
	ERR_REQ_FIELD,
	ERR_COUNT
}	t_error;

typedef enum e_field_type
{
	FIELD_INT,
	FIELD_FLOAT,
	FIELD_V3F,
	FIELD_COL,
	FIELD_ENUM
}	t_f_type;

typedef enum e_field_state
{
	EMPTY		= 0x0,
	FILLED		= 0x1,
	HIDDEN		= 0x2
}	t_f_state;

typedef struct s_field
{
	const char	*name;
	void		*val;
	t_f_type	type;
	t_v2f		limit;
	t_f_state	state;
	union
	{
		t_conv_to_enum	to_enum;
	};
}	t_field;

// Token utilities
bool		validate_tokens(const char *first_token, const char *line);

/* ---------------------Core--------------------- */
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
// material.c
bool		parse_material(char **tokens, t_scene *scene);
// texture.c
void		init_texture_fields(t_field *tex_fields, t_tex *tex);

/* ---------------------Objects--------------------- */
// sphere.c
bool		parse_diameter(float *out, const char *str);
bool		parse_sphere(char **tokens, t_scene *scene);
// plane.c
bool		parse_plane(char **tokens, t_scene *scene);
// cylinder.c
bool		parse_cylinder(char **tokens, t_scene *scene);
// triangle.c
bool		parse_triangle(char **tokens, t_scene *scene);

/* ---------------------Utils--------------------- */
// string_utils.c
void		clean_spaces(char *str);
size_t		token_count_in_str(const char *str);
bool		validate_commas(const char *str);

// vector_utils.c
bool		parse_v3f(t_v3f *v3f, const char *str);
bool		parse_col(t_v3f *color, const char *str);
bool		parse_dir(t_v3f *dir, const char *str);
bool		parse_floats(const char **tokens, float *out,
				size_t count, const char *ctx);
bool		parse_ints(const char **tokens, int *out,
				size_t count, const char *ctx);

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
bool		parse_float(float *out, const char *str,
				t_v2f range, const char *token);
bool		parse_int(int *out, const char *str,
				t_v2f range, const char *token);

// error.c
void		print_error(t_error type, const char *ctx, const char *value);
void		exit_err(t_error type, const char *ctx, const char *value);

// cleanup.c
void		free_tokens(char **tokens);
void		cleanup_gnl(char *line, int fd);
void		free_material(void *ptr);
void		cleanup_scene(t_scene *scene);

// field.c
t_field		init_field(const char *name, void *mem, t_f_type type, t_v2f lim);
bool		is_field(const char *token, const char *field_name,
				const char **value);
bool		parse_fields(t_field *fields, int count, char **tokens);

// material_utils.c
t_mat		*find_or_create_material(t_vector *materials, const char *m_name);
int			is_valid_material_name(const char *m_name);
bool		assign_material(t_obj *obj, t_vector *materials,
				const char *m_name);
t_mat_type	get_mat_type(const char *value);
#endif
