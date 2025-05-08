/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:37:49 by bewong            #+#    #+#             */
/*   Updated: 2025/05/08 22:37:49 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool parse_map(t_parser *parser, const char *file)
{
	int		fd;
	char	*line;
	char	**tokens;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (false);
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break;
		if (line[0] != '#' && line[0] != '\n')
		{
			tokens = ft_split(line, ' ');
			if (tokens == NULL)
				return (free(line), false);
			if (!parse_scene_element(tokens, &parser->scene))
			{
				ft_split_free(tokens);
				free(line);
				return (false);
			}
			ft_split_free(tokens);
		}
		free(line);
	}
	close(fd);
	return (true);
}

t_parser *init_parser(void)
{
	t_parser *parser = malloc(sizeof(t_parser));
	if (!parser)
		return NULL;

	parser->input_file = NULL;
	parser->current_line = NULL;
	parser->line_buffer_size = 0;
	parser->scene = malloc(sizeof(t_scene));
	if (!parser->scene)
	{
		free(parser);
		return NULL;
	}

	memset(parser->scene, 0, sizeof(t_scene));

	parser->config.filename = NULL;
	parser->config.line_number = 0;
	parser->config.has_camera = false;
	parser->config.has_ambient = false;

	parser->last_error = PARSE_OK;

	return parser;
}

void cleanup_parser(t_parser *parser)
{
	if (!parser)
		return;

	free(parser->scene->point_lights);
	free(parser->scene->spheres);
	free(parser->scene->planes);
	free(parser->scene->cylinders);
	free(parser->scene);

	if (parser->input_file)
		fclose(parser->input_file);
	free(parser->current_line);
	free(parser->config.filename);
	free(parser);
}


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "scene.h"
#include "vector.h"


bool parse_vector(char *str, t_v3f *vec)
{
	char *token;
	int i = 0;

	token = strtok(str, ",");
	while (token && i < 3)
	{
		vec->v[i] = atof(token);

		if (i < 3 && (vec->v[i] < -1.0f || vec->v[i] > 1.0f))
			return false;

		token = strtok(NULL, ",");
		i++;
	}

	return (i == 3);
}

bool parse_color(char *str, t_col32 *color)
{
	char *token;
	int i = 0;
	int color_values[3];

	token = strtok(str, ",");
	while (token && i < 3)
	{
		color_values[i] = atoi(token);

		if (color_values[i] < 0 || color_values[i] > 255)
			return false;

		token = strtok(NULL, ",");
		i++;
	}

	if (i == 3)
	{
		color->r = color_values[0];
		color->g = color_values[1];
		color->b = color_values[2];
		return true;
	}

	return false;
}

bool parse_ambient_light(char **tokens, t_light *ambient_light)
{
	if (tokens[1] == NULL || tokens[2] == NULL)
		return false;
	float ratio = atof(tokens[1]);
	if (ratio < 0.0f || ratio > 1.0f)
		return false;
	ambient_light->intensity = ratio;
	if (!parse_color(tokens[2], &ambient_light->col))
		return false;

	ambient_light->type = LIGHT_AMBIENT;
	return true;
}

bool parse_camera(char **tokens, t_camera *camera)
{
	if (tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL)
		return false;

	if (!parse_vector(tokens[1], &camera->t.pos))
		return false;
	if (!parse_vector(tokens[2], &camera->t.dir))
		return false;
	float fov = atof(tokens[3]);
	if (fov < 0.0f || fov > 180.0f)
		return false;
	camera->fov = fov;

	return true;
}

bool parse_point_light(char **tokens, t_light *light)
{
	if (tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL)
		return false;
	if (!parse_vector(tokens[1], &light->pos))
		return false;
	float intensity = atof(tokens[2]);
	if (intensity < 0.0f || intensity > 1.0f)
		return false;
	light->intensity = intensity;
	if (!parse_color(tokens[3], &light->col))
		return false;
	light->type = LIGHT_SPOT;
	return true;
}

bool parse_sphere(char **tokens, t_sphere *sphere)
{
	if (tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL)
		return false;
	if (!parse_vector(tokens[1], &sphere->t.pos))
		return false;
	float diameter = atof(tokens[2]);
	if (diameter <= 0.0f)
		return false;
	sphere->radius = diameter / 2.0f;
	if (!parse_color(tokens[3], &sphere->r.col))
		return false;

	return true;
}

bool parse_plane(char **tokens, t_plane *plane)
{
	if (tokens[1] == NULL || tokens[2] == NULL || tokens[3] == NULL)
		return false;
	if (!parse_vector(tokens[1], &plane->t.pos))
		return false;
	if (!parse_vector(tokens[2], &plane->t.dir))
		return false;
	if (!parse_color(tokens[3], &plane->r.col))
		return false;

	return true;
}

bool parse_cylinder(char **tokens, t_cylinder *cylinder)
{
	if (tokens[1] == NULL || tokens[2] == NULL ||
		tokens[3] == NULL || tokens[4] == NULL || tokens[5] == NULL)
		return false;
	if (!parse_vector(tokens[1], &cylinder->t.pos))
		return false;
	if (!parse_vector(tokens[2], &cylinder->t.dir))
		return false;
	float diameter = atof(tokens[3]);
	if (diameter <= 0.0f)
		return false;
	cylinder->radius = diameter / 2.0f;
	float height = atof(tokens[4]);
	if (height <= 0.0f)
		return false;
	cylinder->height = height;
	if (!parse_color(tokens[5], &cylinder->r.col))
		return false;
	if (!parse_color(tokens[5], &cylinder->r.col))
		return false;

	return true;
}

bool parse_scene_element(char **tokens, t_scene *scene)
{
	if (strcmp(tokens[0], "A") == 0)
		return parse_ambient_light(tokens, &scene->ambient_light);
	else if (strcmp(tokens[0], "C") == 0)
		return parse_camera(tokens, &scene->camera);
	else if (strcmp(tokens[0], "L") == 0)
		return parse_point_light(tokens, &scene->point_lights[scene->num_point_lights++]);
	else if (strcmp(tokens[0], "sp") == 0)
		return parse_sphere(tokens, &scene->spheres[scene->num_spheres++]);
	else if (strcmp(tokens[0], "pl") == 0)
		return parse_plane(tokens, &scene->planes[scene->num_planes++]);
	else if (strcmp(tokens[0], "cy") == 0)
		return parse_cylinder(tokens, &scene->cylinders[scene->num_cylinders++]);

	return false;
}


bool add_object_to_scene(t_scene *scene, void *obj, t_object_type type)
{
	t_object *temp = realloc(scene->objects, 
		(scene->num_objects + 1) * sizeof(t_object));
	
	if (!temp)
		return false;
	
	scene->objects = temp;
	
	t_object new_object;
	new_object.obj = obj;
	new_object.type = type;
	
	switch (type)
	{
		case OBJ_SPHERE:
			new_object.ray_intersect = sphere_ray_intersect;
			break;
		case OBJ_PLANE:
			new_object.ray_intersect = plane_ray_intersect;
			break;
		case OBJ_CYLINDER:
			new_object.ray_intersect = cylinder_ray_intersect;
			break;
		case OBJ_LIGHT:
			new_object.ray_intersect = light_ray_intersect;
			break;
		case OBJ_CAMERA:
			new_object.ray_intersect = NULL;
			break;
		default:
			return false;
	}
	
	scene->objects[scene->num_objects] = new_object;
	scene->num_objects++;
	
	return true;
}

bool parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere *sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return false;
	if (!parse_vector(tokens[1], &sphere->t.pos) ||
		!parse_color(tokens[3], &sphere->r.col))
	{
		free(sphere);
		return false;
	}
	
	if (!add_object_to_scene(scene, sphere, OBJ_SPHERE))
	{
		free(sphere);
		return false;
	}
	
	return true;
}
