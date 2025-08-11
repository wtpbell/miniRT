/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/10 19:43:26 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/11 22:27:55 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
 source: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
*/

void	tri_swap(t_tri *a, t_tri *b) // Replace with int version
{
	t_tri	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	calc_tri_centroid(t_mesh *mesh)
{
	int		i;
	t_tri	*tri;

	i = 0;
	while (i < mesh->tri_count)
	{
		tri = mesh->triangles + i;
		tri->centroid = v3f_scale(
			v3f_add(v3f_add(tri->v0, tri->v1), tri->v2), 1.0f/3.0f);
		++i;
	}
}

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

static bool	is_leaf_node(t_bhv_node *node, uint32_t idx, uint32_t *nodes_used, uint32_t i)
{
	const uint32_t	remaining = i - node[idx].left;
	int				leftIdx;

	if (remaining == 0 || remaining == node[idx].prim_count)
		return (true);
	leftIdx = *nodes_used;
	node[leftIdx].left = node[idx].left;
	node[leftIdx].prim_count = remaining;
	
	node[leftIdx + 1u].left = i;
	node[leftIdx + 1u].prim_count = node[idx].prim_count - remaining;

	node[idx].left = leftIdx;
	node[idx].prim_count = 0;

	// printf("LEFT (%d): %d %d\n", leftIdx, node[leftIdx].left, node[leftIdx].prim_count);
	// printf("RIGHT (%d): %d %d\n", leftIdx + 1u, node[leftIdx + 1u].left, node[leftIdx + 1u].prim_count);

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

static void	subdivide(t_bhv_node *node, uint32_t idx, uint32_t *nodes_used, t_mesh *mesh)
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

	// printf ("WHAT IS %d? %d\n", node[idx].prim_count,  node[idx].left);
	
	subdivide(node, node[idx].left, nodes_used, mesh);
	subdivide(node, node[idx].left + 1u, nodes_used, mesh);
}

void	display_nodes(t_bhv_node *node, uint32_t idx)
{
	if (node[idx].prim_count != 0)
	{
		printf("LEAF: %d %d\n", node[idx].left, node[idx].prim_count);
		return ;
	}
	display_nodes(node, node[idx].left);
	display_nodes(node, node[idx].left + 1);
}

t_bhv_node	*construct_bvh(t_mesh *mesh)
{
	t_bhv_node	*root;
	uint32_t	nodes_used;

	printf("tri count: %d\n", mesh->tri_count);
	root = malloc((mesh->tri_count * 2 - 1) * sizeof(t_bhv_node));
	if (root == NULL)
		return (NULL);
	nodes_used = 1;
	root->left = 0;
	root->prim_count = mesh->tri_count;
	calc_tri_centroid(mesh);
	update_node_bounds(root, mesh);
	subdivide(root, 0, &nodes_used, mesh);
	printf("nodes: %d\n", nodes_used);
	display_nodes(root, 0);
	return (root);
}
