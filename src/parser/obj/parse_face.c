/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_face.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/07 18:08:20 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/07 18:56:50 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#define MAX_I				99999
#define MIN_I				0

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

static bool	parse_set(char *token, int *indices, int curr, int count)
{
	const t_v2f	lim = init_v2f(MIN_I, MAX_I);
	char		*subtoken;

	while (true)
	{
		subtoken = rt_strtok(NULL, "/", &token);
		if (subtoken == NULL)
			break ;
		if (!parse_int(indices + curr, subtoken, lim, subtoken))
			return (false);
		curr += 1 + (*token == '/');
	}
	if (!is_valid_set(indices, indices + (count * 3)))
		return (false);
	return (true);
}

bool	parse_face(char *token, t_vector *f)
{
	char	*subtoken;
	int		*indices;
	int		count;

	indices = ft_calloc(MAX_VERT_PER_SET, sizeof(int));
	if (indices == NULL)
		return (false);
	count = 0;
	while (true)
	{
		subtoken = rt_strtok(NULL, WHITESPACE, &token);
		if (subtoken == NULL)
			break ;
		if (!is_valid_face_syntax(subtoken)
			|| !parse_set(subtoken, indices, count * MAX_VERT_PER_FACE, count)
			|| ++count > MAX_VERT_PER_FACE)
			return (free(indices), false);
	}
	if (!vector_add(f, indices))
		return (free(indices), false);
	return (count == MAX_VERT_PER_FACE);
}
