/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   noise.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/25 23:03:21 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/29 16:38:39 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "random.h"

#define VERTS		256
#define TABLE_SIZE	512

static const int	g_verts = VERTS;
static const int	g_mask = VERTS - 1;
static float		g_noise[VERTS];
static int			g_hashtable[TABLE_SIZE];

void	init_value_noise(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < g_verts)
	{
		g_noise[i] = frandom();
		g_hashtable[i] = i;
		++i;
	}
	i = 0;
	while (i < g_verts)
	{
		j = (int)(frandom() * VERTS) & g_mask;
		ft_swapi(&g_hashtable[i], &g_hashtable[j]);
		g_hashtable[i + g_verts] = g_hashtable[i];
		++i;
	}
}

float	eval_noise2d(t_v2f point)
{
	t_v2i	v2i;
	t_v2i	min;
	t_v2i	max;
	t_v2f	t;
	t_v4f	corners;

	v2i.x = floorf(point.x);
	v2i.y = floorf(point.y);
	min.x = v2i.x & g_mask;
	min.y = v2i.y & g_mask;
	max.x = (min.x + 1) & g_mask;
	max.y = (min.y + 1) & g_mask;
	corners.x = g_noise[g_hashtable[g_hashtable[min.x] + min.y]];
	corners.y = g_noise[g_hashtable[g_hashtable[max.x] + min.y]];
	corners.z = g_noise[g_hashtable[g_hashtable[min.x] + max.y]];
	corners.w = g_noise[g_hashtable[g_hashtable[max.x] + max.y]];
	t.x = smoothstep(point.x - v2i.x);
	t.y = smoothstep(point.y - v2i.y);
	return (lerpf(lerpf(corners.x, corners.y, t.x),
		lerpf(corners.z, corners.w, t.x), t.y));
}
