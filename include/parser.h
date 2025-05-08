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

#include "parser.h"

typedef struct s_scene
{
	t_light		ambient_light;
	t_light		*point_lights;
	size_t		num_point_lights;
	t_camera	camera;
	t_object	*objects;
	size_t		num_objects;
}	t_scene;


# include <stdbool.h>
# include <stddef.h>
# include "scene.h"

typedef enum e_parse_error
{
	PARSE_OK,
	PARSE_ERROR_INVALID_ARGS,
	PARSE_ERROR_INVALID_FORMAT,
	PARSE_ERROR_OUT_OF_RANGE,
	PARSE_ERROR_MEMORY,
	PARSE_ERROR_FILE_READ
} t_parse_error;


typedef struct s_parse_config
{
	char	*filename;
	int		line_number;
	bool	has_camera;
	bool	has_ambient;
} t_parse_config;

typedef struct s_parser
{
	FILE			*input_file;
	char			*current_line;
	size_t			line_buffer_size;
	t_scene			scene;
	t_parse_config	config;
	t_parse_error	last_error;
} t_parser;

bool		parse_file(t_parser *parser, const char *filename);
bool		parse_line(t_parser *parser, char *line);
bool		validate_scene(t_parser *parser);
bool		parse_ambient_light(t_parser *parser, char **tokens);
bool		parse_camera(t_parser *parser, char **tokens);
bool		parse_point_light(t_parser *parser, char **tokens);
bool		parse_sphere(t_parser *parser, char **tokens);
bool		parse_plane(t_parser *parser, char **tokens);
bool		parse_cylinder(t_parser *parser, char **tokens);
bool		parse_vector(char *str, t_v3f *vec);
bool		parse_color(char *str, t_col32 *color);
void		cleanup_parser(t_parser *parser);
t_parser	*init_parser(void);

#endif // PARSER_H

