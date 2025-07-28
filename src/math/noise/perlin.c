/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/29 16:47:20 by jboon         #+#    #+#                 */
/*   Updated: 2025/07/28 10:43:40 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "random.h"

#define VERTS		256
#define TABLE_SIZE	512

static const int	g_verts = VERTS;
static const int	g_mask = VERTS - 1;
static t_v3f		g_gradients[VERTS];
static int			g_hashtable[TABLE_SIZE];

t_v3f	frand_sphere(void)
{
	float	theta;
	float	phi;

	theta = acosf(2.0f * frandom() - 1.0f);
	phi = frandom() * TAU;
	return (init_v3f(
		cosf(phi) * sinf(theta),
		sinf(phi) * sinf(theta),
		cosf(theta)
	));
}

inline int	irand(int min, int max)
{
	return (min + floorf(frandom() * (max - min)));
}

void	init_perlin(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < g_verts)
	{
		g_gradients[i] = frand_sphere();
		g_hashtable[i] = i;
		++i;
	}
	i = 0;
	while (i < g_verts)
	{
		j = irand(0, g_verts * 4) & g_mask;
		ft_swapi(&g_hashtable[i], &g_hashtable[j]);
		g_hashtable[i + g_verts] = g_hashtable[i];
		++i;
	}
}

inline static int hash(int x, int y)
{
	return(g_hashtable[g_hashtable[x] + y]);
}

/**
 * source: https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/perlin-noise-part-2/perlin-noise.html
 */
float	perlin(t_v2f point)
{
	t_v2i	p_i0;
	t_v2i	p_i1;
	t_v2f	p_t;
	t_v2f	travel;
	t_v3f	corners[4];
	t_v3f	points[4];
	t_v2f	vals;

	p_i0.x = ((int)floorf(point.x)) & g_mask;
	p_i0.y = ((int)floorf(point.y)) & g_mask;
	p_i1.x = (p_i0.x + 1) & g_mask;
	p_i1.y = (p_i0.y + 1) & g_mask;
	p_t.x = modulo(point.x);
	p_t.y = modulo(point.y);

	travel.u = smoothstep(p_t.x);
	travel.v = smoothstep(p_t.y);

	corners[0] = g_gradients[hash(p_i0.x, p_i0.y)];	// bottom left
	corners[1] = g_gradients[hash(p_i1.x, p_i0.y)];	// bottom right
	corners[2] = g_gradients[hash(p_i0.x, p_i1.y)];	// top left
	corners[3] = g_gradients[hash(p_i1.x, p_i1.y)];	// top right

	points[0] = init_v3f(p_t.x, p_t.y, 0.0f);
	points[1] = init_v3f(p_t.x - 1.0f, p_t.y, 0.0f);
	points[2] = init_v3f(p_t.x, p_t.y - 1.0f, 0.0f);
	points[3] = init_v3f(p_t.x - 1.0f, p_t.y - 1.0f, 0.0f);

	vals.x = lerpf(v3f_dot(corners[0], points[0]), v3f_dot(corners[1], points[1]), travel.u);
	vals.y = lerpf(v3f_dot(corners[2], points[2]), v3f_dot(corners[3], points[3]), travel.u);

	return (lerpf(vals.x, vals.y, travel.v));
}
