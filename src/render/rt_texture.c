/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rt_texture.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/27 11:54:25 by jboon         #+#    #+#                 */
/*   Updated: 2025/06/27 15:52:12 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "rt_math.h"
#include "color.h"

// texture tiling: texture can be repeated multiple times on the object
// 1. scale the UVs to repeat the texture multiple times on the object
// 2. use fractional part to sample within the single texture
// e.g.UV: 0.9, 0.95, 1.0, 1.05, 1.1 Fractional: 0.9, 0.95, 0.0, 0.05, 0.1
t_v3f	sample_texture(const mlx_texture_t *tex, const t_v2f uv, const t_v3f mod)
{
	t_col32	x;
	t_col32	y;
	t_col32	idx;

	x = (t_col32)(modulo(uv.u * mod.x) * (tex->width - 1));
	y = (t_col32)(modulo(uv.v * mod.y) * (tex->height - 1));
	idx = (y * tex->width + x) * tex->bytes_per_pixel;
	return (col32_to_v3f(init_col32(
				tex->pixels[idx],
				tex->pixels[idx + 1],
				tex->pixels[idx + 2],
				tex->pixels[idx + 3]
			)));
}
