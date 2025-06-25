/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:18:18 by bewong            #+#    #+#             */
/*   Updated: 2025/06/25 11:18:18 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include "material.h"
#include <stdio.h>
#include "rt_math.h"

t_v3f	sample_texture(const t_v2f *texcoord, const t_tex *tex, t_v3f prim_col)
{
	t_v2f	uv = *texcoord;

	(void)prim_col;
	uv = *texcoord;
	if (tex->type == TEX_SOLID)
		return (solid_pattern(&uv, tex, tex->col));
	else if (tex->type == TEX_CHECKER)
		return (checker_pattern(&uv, tex, tex->col));
	else if (tex->type == TEX_IMAGE)
		return (image_pattern(&uv, tex, tex->col));
	return (g_v3f_one);
}

mlx_texture_t *load_png_texture(const char *path)
{
	int				fd;
	mlx_texture_t	*texture;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror("open texture file"), NULL);
	close(fd);
	texture = mlx_load_png(path);
	if (!texture)
		return (perror("load png texture"), NULL);
	return (texture);
}

void	cleanup_texture(t_tex *tex)
{
	if (!tex)
		return ;
	if (tex->tex)
	{
		mlx_delete_texture(tex->tex);
		tex->tex = NULL;
	}
	tex->type = TEX_SOLID;
	tex->scale_rot = init_v3f(1.0f, 1.0f, 0.0f);
	tex->col = g_v3f_one;
}

bool	load_texture(t_tex *tex, const char *path)
{
	mlx_texture_t	*mlx_tex;

	mlx_tex = load_png_texture(path);
	if (!mlx_tex)
		return (false);
	if (tex->tex)
		mlx_delete_texture(tex->tex);
	tex->type = TEX_IMAGE;
	tex->tex = mlx_tex;
	if (tex->scale_rot.x <= 0.0f || tex->scale_rot.y <= 0.0f)
		tex->scale_rot = init_v3f(1.0f, 1.0f, 0.0f);
	return (true);
}
