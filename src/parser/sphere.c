/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:35:28 by bewong            #+#    #+#             */
/*   Updated: 2025/05/10 14:35:28 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse_sphere(char **tokens, t_vector *objects)
{
	t_sphere *sphere;
	float diameter;
	int count;

	count = 0;
	while (tokens[count] != NULL)
		count++;
	printf("Parsing sphere. Token count: %d\n", count);
	if (count != 4)
	{
		*error() = ERR_SPHERE_ARGS;
		return (false);
	}
	sphere = ft_calloc(1, sizeof(t_sphere));
	if (!sphere)
		return (false);
	if (!parse_v3f(&sphere->t.pos, tokens[1]) ||
		!parse_float(&diameter, tokens[2]) ||
		!parse_col(&sphere->r.col, tokens[3]))
		return (free(sphere), false);
	sphere->radius = diameter / 2;
	ft_memset(&sphere->t.dir, 0, sizeof(t_v3f));
	if (!vector_add(objects, sphere))
		return (free(sphere), false);
	return (true);
}
