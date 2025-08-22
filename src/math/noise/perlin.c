/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   perlin.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/29 16:47:20 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 15:35:24 by jboon         ########   odam.nl         */
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

void	init_perlin(void)
{
	int	i;
	int	j;

	i = 0;
	seed_rand(109348543);
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

static inline int	hash(int x, int y)
{
	return (g_hashtable[g_hashtable[x] + y]);
}

/**
 * source: scratchapixel - perlin noise
 * Ken Perlin: https://dl.acm.org/doi/pdf/10.1145/325334.325247
 */
float	perlin(t_v2f point)
{
	t_v2f	p_t;
	t_v2f	t;
	t_v2i	p_i[2];
	t_v3f	cors[4];
	t_v3f	pnts[4];

	p_i[0].x = ((long)floorf(point.x)) & g_mask;
	p_i[0].y = ((long)floorf(point.y)) & g_mask;
	p_i[1].x = (p_i[0].x + 1) & g_mask;
	p_i[1].y = (p_i[0].y + 1) & g_mask;
	p_t = v2f_fract(point);
	t.u = perlin_smoothstep(p_t.x);
	t.v = perlin_smoothstep(p_t.y);
	cors[0] = g_gradients[hash(p_i[0].x, p_i[0].y)];
	cors[1] = g_gradients[hash(p_i[1].x, p_i[0].y)];
	cors[2] = g_gradients[hash(p_i[0].x, p_i[1].y)];
	cors[3] = g_gradients[hash(p_i[1].x, p_i[1].y)];
	pnts[0] = init_v3f(p_t.x, p_t.y, 0.0f);
	pnts[1] = init_v3f(p_t.x - 1.0f, p_t.y, 0.0f);
	pnts[2] = init_v3f(p_t.x, p_t.y - 1.0f, 0.0f);
	pnts[3] = init_v3f(p_t.x - 1.0f, p_t.y - 1.0f, 0.0f);
	return (lerpf(
			lerpf(v3f_dot(cors[0], pnts[0]), v3f_dot(cors[1], pnts[1]), t.u),
			lerpf(v3f_dot(cors[2], pnts[2]), v3f_dot(cors[3], pnts[3]), t.u),
			t.v));
}
