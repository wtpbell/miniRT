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

#include "parser.h"

void	cleanup_texture(t_tex *tex)
{
	free(tex->tex_path);
	if (tex->tex)
		mlx_delete_texture(tex->tex);
	tex->tex_path = NULL;
	tex->tex = NULL;
	tex->type = TEX_SOLID;
	tex->scale_rot = init_v3f(1.0f, 1.0f, 0.0f);
	tex->col = g_v3f_one;
}

bool	load_texture(t_tex *tex, const char *path)
{
	mlx_texture_t	*mlx_tex;

	mlx_tex = mlx_load_png(path);
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

void	assign_textures(t_mat *mat)
{
	if (!str_is_empty(mat->texture.tex_path))
	{
		if (!load_texture(&mat->texture, mat->texture.tex_path))
			mat->texture.type = TEX_SOLID;
	}
	if (!str_is_empty(mat->bump_map.tex_path))
	{
		if (load_bump_map(mat, mat->bump_map.tex_path))
			mat->bump_map.type = TEX_IMAGE;
	}
}
