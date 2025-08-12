/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/10 19:43:26 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/12 16:59:44 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	update_node_bounds(t_bhv_node *node, t_mesh *mesh)
{
	uint32_t	i;
	t_tri		*tri;
	t_v3f		min;
	t_v3f		max;

	i = 0;
	min = v3f_scale(g_v3f_one, 1e30f);
	max = v3f_scale(g_v3f_one, -1e30f);
	while (i < node->prim_count)
	{
		tri = mesh->triangles + node->left + i;
		min.x = fminf(fminf(fminf(min.x, tri->v0.x), tri->v1.x), tri->v2.x);
		min.y = fminf(fminf(fminf(min.y, tri->v0.y), tri->v1.y), tri->v2.y);
		min.z = fminf(fminf(fminf(min.z, tri->v0.z), tri->v1.z), tri->v2.z);
		max.x = fmaxf(fmaxf(fmaxf(max.x, tri->v0.x), tri->v1.x), tri->v2.x);
		max.y = fmaxf(fmaxf(fmaxf(max.y, tri->v0.y), tri->v1.y), tri->v2.y);
		max.z = fmaxf(fmaxf(fmaxf(max.z, tri->v0.z), tri->v1.z), tri->v2.z);
		++i;
	}
	node->box.min = min;
	node->box.max = max;
}

static bool	is_leaf_node(t_bhv_node *node, uint32_t idx, uint32_t *nodes_used,
	uint32_t i)
{
	const uint32_t	remaining = i - node[idx].left;
	int				left_idx;

	if (remaining == 0 || remaining == node[idx].prim_count)
		return (true);
	left_idx = *nodes_used;
	node[left_idx].left = node[idx].left;
	node[left_idx].prim_count = remaining;
	node[left_idx + 1u].left = i;
	node[left_idx + 1u].prim_count = node[idx].prim_count - remaining;
	node[idx].left = left_idx;
	node[idx].prim_count = 0;
	(*nodes_used) += 2;
	return (false);
}

static int	get_longest_axis(t_v3f extent)
{
	int	axis;

	axis = 0;
	if (extent.y > extent.x)
		axis = 1;
	if (extent.z > extent._axis[axis])
		axis = 2;
	return (axis);
}

static void	subdivide(t_bhv_node *node, uint32_t idx, uint32_t *nodes_used,
	t_mesh *mesh)
{
	const t_v3f	extent = v3f_sub(node[idx].box.max, node[idx].box.min);
	const int	axis = get_longest_axis(extent);
	int			i;
	int			j;
	float		split_pos;

	split_pos = node[idx].box.min._axis[axis] + extent._axis[axis] * 0.5f;
	i = node[idx].left;
	j = i + node[idx].prim_count - 1;
	while (i <= j)
	{
		if (mesh->triangles[i].centroid._axis[axis] < split_pos)
			++i;
		else
		{
			tri_swap(mesh->triangles + i, mesh->triangles + j);
			--j;
		}
	}
	if (is_leaf_node(node, idx, nodes_used, i))
		return ;
	update_node_bounds(node + node[idx].left, mesh);
	update_node_bounds(node + (node[idx].left + 1u), mesh);
	subdivide(node, node[idx].left, nodes_used, mesh);
	subdivide(node, node[idx].left + 1u, nodes_used, mesh);
}

/*
 source: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
*/
t_bhv_node	*construct_bvh(t_mesh *mesh)
{
	t_bhv_node	*root;
	uint32_t	nodes_used;

	root = malloc((mesh->tri_count * 2 - 1) * sizeof(t_bhv_node));
	if (root == NULL)
		return (NULL);
	nodes_used = 1;
	root->left = 0;
	root->prim_count = mesh->tri_count;
	calc_tri_centroid(mesh->triangles, mesh->tri_count);
	update_node_bounds(root, mesh);
	subdivide(root, 0, &nodes_used, mesh);
	return (root);
}
