/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   obj.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/02 15:58:08 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/06 22:40:15 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// Need a struct to store the obj file into
// Need temporary vector lists for storing the vertices, texture coordinates and normals;

#include "parser.h"
#include "errno.h"

#define WHITESPACE " \f\r\t\v\n"
#define MAX_VAL		1.0e20f
#define MIN_VAL		-MAX_VAL
#define MAX_I		99999
#define MIN_I		-MAX_I

// TODO: Better error messages
static bool	init_obj_file(t_obj_file *obj_file)
{
	ft_bzero(obj_file, sizeof(obj_file));
	if (!vector_init(&obj_file->v, 3)
		|| !vector_init(&obj_file->vt, 3)
		|| !vector_init(&obj_file->vn, 3)
		|| !vector_init(&obj_file->f, 3))
	{
		vector_free(&obj_file->v, free);
		vector_free(&obj_file->vt, free);
		vector_free(&obj_file->vn, free);
		return (false);
	}
	return (true);
}

static void	free_obj_file(t_obj_file *obj_file)
{
	vector_free(&obj_file->v, free);
	vector_free(&obj_file->vt, free);
	vector_free(&obj_file->vn, free);
	vector_free(&obj_file->f, free);
}

static inline void assign_v3f(t_v3f *v, t_vector *cont, int i)
{
	// TODO: If index isn't valid have it fail or print error message (missing index)
	if (vector_get(cont, i) != NULL)
		*v = *((t_v3f *)cont->items[i]);
}

// temp function
static inline t_v3f	get_normal(t_v3f v0, t_v3f v1, t_v3f v2)
{
	return (v3f_norm(v3f_cross(v3f_sub(v1, v0), v3f_sub(v2, v0))));
}

static void	construct_mesh_aabb(t_mesh *mesh)
{
	int		i;
	t_tri	*tri;
	t_v3f	min;
	t_v3f	max;

	i = 0;
	min = g_v3f_zero;
	max = g_v3f_zero;
	while (i < mesh->triangles.size)
	{
		tri = (t_tri *)mesh->triangles.items[i];

		min.x = fminf(fminf(fminf(min.x, tri->v0.x), tri->v1.x), tri->v2.x);
		min.y = fminf(fminf(fminf(min.y, tri->v0.y), tri->v1.y), tri->v2.y);
		min.z = fminf(fminf(fminf(min.z, tri->v0.z), tri->v1.z), tri->v2.z);

		max.x = fmaxf(fmaxf(fmaxf(max.x, tri->v0.x), tri->v1.x), tri->v2.x);
		max.y = fmaxf(fmaxf(fmaxf(max.y, tri->v0.y), tri->v1.y), tri->v2.y);
		max.z = fmaxf(fmaxf(fmaxf(max.z, tri->v0.z), tri->v1.z), tri->v2.z);
		++i;
	}
	mesh->box.min = min;
	mesh->box.max = max;
}

// TODO: vn and vt are optional; If none are supplied within the obj file then it must be calcaluted.
static bool	load_obj_into_mesh(t_obj_file *obj_file, t_mesh *mesh, t_mat4x4 local)
{
	int		i;
	int		*indices;
	t_tri	*tri;

	i = 0;
	while (i < obj_file->f.size)
	{
		tri = ft_calloc(1, sizeof(t_tri));
		if (tri == NULL)
			return (false);
		indices = (int *)obj_file->f.items[i];
		assign_v3f(&tri->v0, &obj_file->v, *(indices + 0) - 1);
		assign_v3f(&tri->v1, &obj_file->v, *(indices + 3) - 1);
		assign_v3f(&tri->v2, &obj_file->v, *(indices + 6) - 1);

		if (*(indices + 1) != 0) // No vt were assigned
		{
			assign_v3f(&tri->vt0, &obj_file->vt, *(indices + 1) - 1);
			assign_v3f(&tri->vt1, &obj_file->vt, *(indices + 4) - 1);
			assign_v3f(&tri->vt2, &obj_file->vt, *(indices + 7) - 1);
		}
		else
		{
			generate_uv_vertices(tri, local);
		}

		if (*(indices + 2) == 0) // No vn were assigned
		{
			// Needs to be normalized
			assign_v3f(&tri->vn0, &obj_file->vn, *(indices + 2) - 1);
			assign_v3f(&tri->vn1, &obj_file->vn, *(indices + 5) - 1);
			assign_v3f(&tri->vt2, &obj_file->vn, *(indices + 8) - 1);
		}
		else
		{
			t_v3f norm = get_normal(tri->v0, tri->v1, tri->v2);
			tri->vn0 = norm;
			tri->vn1 = norm;
			tri->vn2 = norm;
		}

		if (!vector_add(&mesh->triangles, tri))
			return (false);
		++i;
	}
	construct_mesh_aabb(mesh);
	printf("%f %f %f || %f %f %f\n", mesh->box.min.x, mesh->box.min.y, mesh->box.min.z, mesh->box.max.x, mesh->box.max.y, mesh->box.max.z);
	printf("triangles: %i\n", mesh->triangles.size);
	return (true);
}

static int	to_v3f(t_v3f *dst, char *saveptr, t_v2f lim)
{
	int		count;
	char	*token;

	count = 0;
	while (true)
	{
		token = rt_strtok(NULL, WHITESPACE, &saveptr);
		if (token == NULL)
			break ;
		if (count < 3 && !parse_float(dst->_axis + count, token, lim, token))
			return (0);
		++count;
	}
	return (count);
}

static bool	copy_v3f_to(const t_v3f point, t_vector *v)
{
	t_v3f	*copy;

	copy = malloc(sizeof(t_v3f));
	if (copy == NULL)
		return (false);
	*copy = point;
	if (vector_add(v, copy))
		return (true);
	return (free(copy), false);
}

static bool	parse_vertex(char *str, t_vector *v)
{
	int		count;
	t_v3f	point;

	count = to_v3f(&point, str, init_v2f(MIN_VAL, MAX_VAL));
	if (count != 3 && count != 4)
		return (false);
	return (copy_v3f_to(point, v));
}

static bool	parse_vertex_texcoord(char *str, t_vector *vt)
{
	int		count;
	t_v3f	point;

	count = to_v3f(&point, str, init_v2f(0.0f, 1.0f));
	if (count < 1 || count > 3)
		return (false);
	while (count < 3)
	{
		point._axis[count] = 0.0f;
		++count;
	}
	return (copy_v3f_to(point, vt));
}

static bool	parse_vertex_normal(char *str, t_vector *vn)
{
	int		count;
	t_v3f	point;

	count = to_v3f(&point, str, init_v2f(MIN_VAL, MAX_VAL));
	if (count != 3)
		return (false);
	return (copy_v3f_to(point, vn));
}

static bool	is_valid_face_syntax(const char *face)
{
	return (*face != '/' && rt_count_occ(face, '/') < 3);
}

static bool	is_valid_set(const int *first, const int *second)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if ((first[i] != 0) != (second[i] != 0))
			return (false);
		++i;
	}
	return (true);
}

static bool	parse_face(char *str, t_vector *f)
{
	const	t_v2f	lim = init_v2f(MIN_I, MAX_I);
	const	int		max = 9;
	char			*token;
	char			*subtoken;
	char			*saveptr;
	int				*indices;
	int				curr;
	int				count;

	indices = ft_calloc(max, sizeof(int)); // A face will have a maximum of 3 points, where each point can have up to 3 indicices seperated by /'s
	if (indices == NULL)
		return (false);
	count = 0;
	while (true)
	{
		token = rt_strtok(NULL, WHITESPACE, &str);
		if (token == NULL)
			break ;

		if (!is_valid_face_syntax(token))
			return (free(indices), false);

		curr = count * 3;
		while (true) // We can assume there won't be more than 2 /'s
		{
			subtoken = rt_strtok(token, "/", &saveptr);
			if (subtoken == NULL)
				break ;
			if (!parse_int(indices + curr, subtoken, lim, subtoken))
				return (free(indices), false);
			token = NULL;
			curr += 1 + (*saveptr == '/'); // Skip one if vt is not supplied (v//vn)
		}

		// All point sets must match with the first set
		if (!is_valid_set(indices, indices + (count * 3)))
			return (free(indices), false);

		++count;
		if (count > 3) // Cannot have more than 3 points per face (unless we support triangulating of the faces ourselves)
			return (free(indices), false);
	}
	if (!vector_add(f, indices))
		return (free(indices), false);
	return (count == 3);
}

static bool	parse_line(char *line, t_obj_file *obj_file)
{
	char	*token;
	char	*saveptr;

	token = rt_strtok(line, WHITESPACE, &saveptr);
	if (token == NULL || *token == '#')
		return (true);
	if (ft_strcmp(token, "f") == 0)
		return (parse_face(saveptr, &obj_file->f));
	else if (ft_strcmp(token, "v") == 0)
		return (parse_vertex(saveptr, &obj_file->v));
	else if (ft_strcmp(token, "vt") == 0)
		return (parse_vertex_texcoord(saveptr, &obj_file->vt));
	else if (ft_strcmp(token, "vn") == 0)
		return (parse_vertex_normal(saveptr, &obj_file->vn));
	return (true);
}

static bool	parse_obj_file(t_mesh *mesh, t_mat4x4 local)
{
	const char	*file_name = mesh->obj_path;
	char		*line;
	int			fd;
	t_obj_file	obj_file;

	printf("PARSE OBJ: %s\n", file_name);
	fd = open(file_name, O_RDONLY);
	if (fd < 0 || !init_obj_file(&obj_file))
		return (perror("parse mesh"), close(fd), false);
	errno = 0;
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (!parse_line(line, &obj_file))
			return (cleanup_gnl(line, fd), free_obj_file(&obj_file), false);
		free(line);
	}
	close (fd);
	get_next_line(-1);
	if (errno != 0)
		return (perror("parse mesh"), free_obj_file(&obj_file), false);
	if (!load_obj_into_mesh(&obj_file, mesh, local))
		return (free_obj_file(&obj_file), false);
	return (free_obj_file(&obj_file), true);
}

bool	handle_mesh_obj(t_scene *scene)
{
	t_obj	*obj;
	int		i;
	int		size;

	i = 0;
	size = scene->objects.size;
	while (i < size)
	{
		obj = (t_obj *)scene->objects.items[i];
		// TODO: Cache the obj files for faster re-use
		if (obj->type == OBJ_MESH && !parse_obj_file(&obj->mesh, obj->t.to_obj))
			return (false);
		++i;
	}
	return (true);
}
